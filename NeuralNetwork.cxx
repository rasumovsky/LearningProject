////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: NeuralNetwork.cxx                                                   //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 24/02/2015                                                          //
//                                                                            //
//  This class creates a neural network using the Neuron and Axon classes.    //
//  It contains methods for constructing, training, and testing a BNPP.       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "NeuralNetwork.h"

/**
   NeuralNetwork constructor.
   @param nInputs - The number of input variables.
   @param nHiddenLayers- The number of internal network layers.
   @param nNodesPerLayer - The number of nodes per hidden layer.
 */
NeuralNetwork::NeuralNetwork(int nInputs, int nHiddenLayers,
			     int nNodesPerLayer) {
  m_nInputs = nInputs;
  m_nHiddenLayers = nHiddenLayers;
  m_nNodesPerLayer = nNodesPerLayer;
  m_axons.clear();
  m_neurons.clear();
  
  // Add the first (visible) layer:
  addLayer(0, m_nInputs);  
  // Hidden layer indices start at 1:
  for (int i_i = 1; i_i <= m_nHiddenLayers; i_i++) {
    addLayer(i_i, m_nNodesPerLayer);
  }
  // The output layer:
  addLayer(m_nHiddenLayers+1, 1);
}

/**
   -----------------------------------------------------------------------------
   Add a layer to the neural network and connect to the preceding layer (if it
   is not the first layer (index = 0). 
   @param layerIndex - The index of the layer. 
   @param nodesPerLayer - The number of nodes per hidden layer.
*/
void NeuralNetwork::addLayer(int layerIndex, int nodesPerLayer) {
  for (int i_n = 0; i_n < nodesPerLayer; i_n++) {
    Neuron *currNeuron = new Neuron(layerIndex, "tanh");
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
}

/**
   -----------------------------------------------------------------------------
   Get a list of Neurons in a particular layer of the network.
   @param layerIndex - The index of the layer.
   @returns - A vector of Neurons in the specified layer.
*/
std::vector<Neuron*> getLayer(int layerIndex) {
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
*/
double getNetworkResponse(std::vector<double> inputs) {
  if ((int)inputs.size() != m_nInputs) {
    std::cout << "NeuralNetwork; ERROR! Wrong size of inputs." << std::endl;
  }

  // NEED TO RETHINK THIS!!!
  // FIRST, VARS SHOULD HAVE RANGE -1,1. 
  // SECOND, INPUTS SHOULD BE NODES OR CONNECTIONS?
  // THIRD, DO YOU NEED TO MULTIPLY WEIGHTS BY PREVIOUS RESPONSE?
  // Set layer zero nodes to 
  std::vector<Neuron*> inputLayer = getLayer(0);
  for (int i_n = 0; i_n < m_nInputs; i_n++) {
    inputLayer[i_n]->setResponse(inputs[i_n], true);
  }
  
  // Get the last layer:
  std::vector<Neuron*> outputLayer = getLayer(m_nHiddenLayers+1);
  return outputLayer[0]->getResponse();
}
