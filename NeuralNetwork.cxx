////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: NeuralNetwork.cxx                                                   //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
//  This class creates a neural network using the Neuron and Axon classes.    //
//  It contains methods for constructing, training, and testing a BNPP.       //
//                                                                            //
//  Outline:                                                                  //
//  - constructor to build network                                            //
//  - randomize weights                                                       //
//  - get network response                                                    //
//  - set targets                                                             //
//  - back-propagation                                                        //
//  - ?                                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "NeuralNetwork.h"

/**
   NeuralNetwork constructor.
   @param nInputs - The number of input variables.
   @param nOutputs - The number of output variables.
   @param nHiddenLayers- The number of internal network layers.
   @param nNodesPerLayer - The number of nodes per hidden layer, excluding bias
 */
NeuralNetwork::NeuralNetwork(int nInputs, int nOutputs, int nHiddenLayers,
			     int nNodesPerLayer) {
  m_nInputs = nInputs;
  m_nOutputs = nOutputs;
  m_nHiddenLayers = nHiddenLayers;
  m_nNodesPerLayer = nNodesPerLayer;
  m_axons.clear();
  m_neurons.clear();
  
  // Add the first (visible) layer:
  addLayer(0, m_nInputs, "linear");  
  // Hidden layer indices start at 1:
  for (int i_i = 1; i_i <= m_nHiddenLayers; i_i++) {
    addLayer(i_i, m_nNodesPerLayer, "sigmoid");
  }
  // The output layer:
  addLayer(m_nHiddenLayers+1, m_nOutputs, "linear");
}

/**
   -----------------------------------------------------------------------------
   Add a layer to the neural network and connect to the preceding layer (if it
   is not the first layer (index = 0). 
   @param layerIndex - The index of the layer. 
   @param nodesPerLayer - The number of nodes per hidden layer.
*/
void NeuralNetwork::addLayer(int layerIndex, int nodesPerLayer, 
			     string function) {
  for (int i_n = 0; i_n < nodesPerLayer; i_n++) {
    Neuron *currNeuron = new Neuron(layerIndex, function);
    if (layerIndex > 0) {
      // Get a list of upstream neurons.
      std::vector<Neuron*> previousLayer = getLayer(layerIndex-1);
      // Create a connection to each.
      for (std::vector<Neuron*>::iterator neuroIter = previousLayer.begin();
	   neuroIter != previousLayer.end(); neuroIter++) {
	Axon *currAxon = new Axon(1.0, *neuroIter, currNeuron);
	m_axons.push_back(currAxon);
      }
    }
    m_neurons.push_back(currNeuron);
  }
  // Then add a bias node (not for input or output layers):
  if (layerIndex > 0 && layerIndex != m_nHiddenLayers+1) {
    Neuron *currNeuron = new Neuron(layerIndex, function);
    currNeuron->setBiasNode(true);
    m_neurons.push_back(currNeuron);
  }
}

/**
   -----------------------------------------------------------------------------
   Get the Neurons that are bias nodes.
*/
std::vector<Neuron*> NeuralNetwork::getBiasNodes() {
  std::vector<Neuron*> result; result.clear();
  for (std::vector<Neuron*>::iterator neuroIter = m_neurons.begin(); 
       neuroIter != m_neurons.end(); neuroIter++) {
    if (neuroIter->isBiasNode()) {
      result.push_back(neuroIter);
    }
  }
  return result;
}

/**
   -----------------------------------------------------------------------------
   Get the Neurons in the input layer.
*/
std::vector<Neuron*> NeuralNetwork::getInputLayer() {
  return getLayer(0);
}

/**
   -----------------------------------------------------------------------------
   Get the Neurons in the output layer.
*/
std::vector<Neuron*> NeuralNetwork::getOutputLayer() {
  return layer(m_nHiddenLayers+1);
}

/**
   -----------------------------------------------------------------------------
   Get a list of Neurons in a particular layer of the network.
   @param layerIndex - The index of the layer.
   @returns - A vector of Neurons in the specified layer.
*/
std::vector<Neuron*> NeuralNetwork::getLayer(int layerIndex) {
  std::vector<Neuron*> layer; layer.clear();
  // Loop over all neurons stored and return the ones with the proper layer idx.
  for (std::vector<Neuron*>::iterator neuroIter = m_neurons.begin();
       neuroIter != m_neurons.end(); neuroIter++) {
    if (neuroIter->getLayerIndex() == layerIndex) layer.push_back(neuroIter);
  }
  return layer;
}

/**
   -----------------------------------------------------------------------------
   Retrieve the network response based on the given inputs.
*/
std::vector<double> NeuralNetwork::getNetworkResponse(std::vector<double> vars){
  if ((int)vars.size() != m_nInputs) {
    std::cout << "NeuralNetwork: ERROR! Wrong size of inputs." << std::endl;
    exit(0);
  }
  
  // FIRST, VARS SHOULD HAVE RANGE -1,1. 

  std::vector<double> result; result.clear();
  
  // First clear the responses in the network. It is recursive, so only need to
  // call on the output nodes, and they will propagate it upstream to ancestors:
  std::vector<Neuron*> outputLayer = getOutputLayer();
  for (std::vector<Neuron*>::iterator neuroIter = outputLayer.begin();
       neuroIter != outputLayer.end(); neuroIter++) {
    neuroIter->clearResponse();
  }
  
  // Then set input layer responses using input variables:
  std::vector<Neuron*> inputLayer = getInputLayer();
  for (int i_n = 0; i_n < m_nInputs; i_n++) {
    inputLayer[i_n]->setResponseWithSum(vars[i_n]);
  }
  
  // Then get the responses of the output layer. Again, this algorithm is
  // recursive, so you only need to call it on the output layers.
  for (std::vector<Neuron*>::iterator neuroIter = outputLayer.begin();
       neuroIter != outputLayer.end(); neuroIter++) {
    result.push_back(neuroIter->getResponse());
  }
  
  // Check that output vector is correct size before returning:
  if ((int)result.size() != m_nOutputs) {
    std::cout << "NeuralNetwork: ERROR! Wrong number of outputs! Expect " 
	      << m_nOutputs << " and find " << result.size() << std::endl;
    exit(0);
  }
  return result;
}

/**
   -----------------------------------------------------------------------------
   Randomize the values of the weights for all connections (Axons) in the 
   network. Useful when starting the training. Note: this relies on rand() from
   <cstdlib>, which can't be reproduced via seeding. Might consider replacing
   with a seeded random number generator in the future.
*/
void NeuralNetwork::randomizeNetworkWeights() {
  // Loop over the axons:
  for (std::vector<Axon*>::iterator axonIter = m_axons.begin();
       axonIter != m_axons.end(); axonIter++) {
    // for each, assign a random weight between -1 and 1:
    int randInt = rand() % 100;// [0,99]
    double normalizedRand = (double)(randInt - 50)/50.0;
    axonIter->setWeight(normalizedRand);
  }
}

/**
   -----------------------------------------------------------------------------
   Set the targets for the output layer.
   @param targets - A vector containing target values.
*/
void NeuralNetwork::setNetworkTargets(std::vector<double> targets) {
  // Check that enough targets were provided:
  if ((int)targets.size() != m_nOutputs) {
    std::cout << "NeuralNetwork: ERROR! Wrong number of target variables!"
	      << std::endl;
    exit(0);
  }
  
  // Then set the target value for each output layer node:
  std::vector<Neuron*> outputLayer = getOutputLayer();
  for (int i_n = 0; i_n < m_nOutputs; i_n++) {
    outputLayer[i_n]->setTarget(targets[i_n]);
  }
}

/**
   -----------------------------------------------------------------------------
   Use back-propagation to update the weights in the network. Assumes training
   has just taken place and network targets have also been set. 
*/
void NeuralNetwork::updateNetworkViaBP() {
  // First clear deltas for input layer:
  std::vector<Neuron*> inputLayer = getInputLayer();
  for (std::vector<Neuron*>::iterator neuroIter = inputLayer.begin(); 
       neuroIter != inputLayer.end(); neuroIter++) {
    neuroIter->clearDelta();
  }
  
  // Then clear deltas for bias nodes (technically might not be necessary):
  std::vector<Neuron*> biasNodes = getBiasNodes();
  for (std::vector<Neuron*>::iterator neuroIter = biasNodes.begin(); 
       neuroIter != biasNodes.end(); neuroIter++) {
    neuroIter->clearDelta();
  }
  
  // Finally call back-propagation algorithm, which is recursive. It finds all
  // of the input and bias nodes and calls getDelta() on them. This getDelta()
  // method is recursive, and calls all downstream getDeltas(). These update
  // the weights of the connections after they have been used for all necessary
  // calculations. Probably only need to call on a single output layer node, but
  // for completeness, we will call it on all of them.
  std::vector<Neuron*> outputLayer = getOutputLayer();
  for (std::vector<Neuron*>::iterator neuroIter = outputLayer.begin(); 
       neuroIter != outputLayer.end(); neuroIter++) {
    neuroIter->backPropagation();
  }
}
