////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Neuron.cxx                                                          //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
//  This class represents a neuron in a neural network. The connections       //
//  are to upstream and downstream nodes.                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Neuron.h"

/**
   -----------------------------------------------------------------------------
   Neuron constructor, with option to set the threshold function:
   @param function - The activation function for the neuron: "sigmoid", "tanh",
   "linear", all defined for x:[-1,+1], y:[-1,+1].
   @param biasNode - True iff this node is a bias node.
*/
Neuron::Neuron(std::string function, bool biasNode) {
  m_biasNode = biasNode;
  m_hasResponse = false;
  m_response = 0.0;
  m_downstreamConnections.clear();
  m_upstreamConnections.clear();
  m_function = function;
  return;
}

/**
   -----------------------------------------------------------------------------
   Add a single downstream connection.
   @param axon - A single downstream connection.
*/
void Neuron::addDownstreamConnection(Axon *axon) {
  if (!axon) {
    std::cout << "Neuron: ERROR! axon is null" << std::endl;
    exit(0);
  }
  m_downstreamConnections->push_back(axon);
}

/**
   -----------------------------------------------------------------------------
   Add multiple downstream connections.
   @param axons - A vector of downstream connections.
*/
void Neuron::addDownstreamConnection(std::vector<Axon*> axons) {
  std::vector<Axon*>::iterator axonIter;
  for (axonIter = axons.begin(); axonIter != axons.end(); axonIter++) {
    addDownstreamConnection(*axonIter);
  }
}

/**
   -----------------------------------------------------------------------------
   Add a single upstream connection.
   @param axon - A single upstream connection.
*/
void Neuron::addUpstreamConnection(Axon *axon) {
  if (!axon) {
    std::cout << "Neuron: ERROR! axon is null" << std::endl;
    exit(0);
  }
  else if (isBiasNode()) {
    std::cout << "Error: cannot add upstream connection to bias Node." 
	      << std::endl;
    exit(0);
  }
  else {
    m_upstreamConnections->push_back(axon);
  }
}

/**
   -----------------------------------------------------------------------------
   Add multiple upstream connections.
   @param axons - A vector of upstream connections.
*/
void Neuron::addUpstreamConnection(std::vector<Axon*> axons) {
  for (std::vector<Axon*>::iterator axonIter = axons.begin();
       axonIter != axons.end(); axonIter++) {
    addUpstreamConnection(*axonIter);
  }
}

/**
   -----------------------------------------------------------------------------
   Clears the response information, which is only stored for speed. Note: this
   algorithm is recursive. It will clear the response for all upstream
   (ancestor) Neurons.
*/
void Neuron::clearResponse() {
  if (m_hasResponse) {
    m_hasResponse = false;
    m_response = 0.0;
    // Then recursively clear ancestor Neurons' responses. 
    std::vector<Axon*> ancestorConnections = getUpstreamConnections();
    for (std::vector<Axon*> axonIter = ancestorConnections.begin();
	 axonIter != ancestorConnections.end(); axonIter++) {
      axonIter->getOriginNeuron->clearResponse();
    }
  }    
}

/**
   -----------------------------------------------------------------------------
   @returns - A vector of downstream connections for the Neuron.
*/
std::vector<Axon*> Neuron::getDownstreamConnections() {
  return m_downstreamConnections;
}

/**
   -----------------------------------------------------------------------------
   @returns - The number of downstream connections for the Neuron.
*/
int Neuron::getNDownstreamConnections() {
  return (int)m_downstreamConnections.size();
}

/**
   -----------------------------------------------------------------------------
   @returns - The number of upstream connections for the Neuron.
*/
int Neuron::getNUpstreamConnections() {
  return (int)m_upstreamConnections.size();
}

/**
   -----------------------------------------------------------------------------
   Evaluate the neuron response based on the current input connection weights.
   @returns - The neuron response, which is 1 for bias nodes, equal to the input
   for input nodes, and is variable for all other nodes. 
*/
double Neuron::getResponse() {
  if (isBiasNode()) {
    m_response = 1.0;
  }
  else if (isInputNode()) {
    if (!m_hasResponse) {
      std::cout << "Neuron: Error! Input node response undefined." << std::endl;
      exit(0);
    }
  }
  else if (!m_hasResponse) {
    m_response = 0.0;
    // Loop over input connections
    for (std::vector<Axon*>::iterator axonIter = m_upstreamConnections.begin();
	 axonIter != m_upstreamConnections.end(); axonIter++) {
      // sum the input weights:
      double inputWeight = axonIter->getWeight();
      double inputResponse = axonIter->getOriginNeuron()->getResponse();
      response += (inputWeight * inputResponse);
    }
    // Then get the result of the threshold function:
    m_response = thresholdFunction(m_response);
  }
  m_hasResponse = true;
  return m_response;
}

/**
   -----------------------------------------------------------------------------
   @returns - A vector of upstream connections for the Neuron.
*/
std::vector<Axon*> Neuron::getUpstreamConnections() {
  return m_upstreamConnections;
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is a (constant) bias node.
*/
bool Neuron::isBiasNode() {
  return m_biasNode();
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is an input node (without predecessors and not bias 
   node).
*/
bool Neuron::isInputNode() {
  if (isBiasNode()) return false;
  else return (getNUpstreamConnections() == 0);
}

/**
   -----------------------------------------------------------------------------
   Check whether this node is an output node (without ancestors).
*/
bool Neuron::isOutputNode() {
  return (getNDownstreamConnections() == 0);
}

/**
   -----------------------------------------------------------------------------
   Evaluate the threshold function to see whether the neuron should fire. The
   threshold functions are defined to be on an interval x:[-1,+1] with an output
   y:[-1,+1].
   @param sum - The sum of weights of the input connections.
   @returns - The response for a given sum and function.
*/
double Neuron::thresholdFunction(double sum) {
  double result = 0;
  if (m_function == "sigmoid") {
    result = (2.0 / (1 + exp(-1.0*sum))) - 1.0;
  }
  else if (m_function == "tanh") {
    result = (exp(sum) - exp(-1.0*sum)) / (exp(sum) + exp(-1.0*sum));
  }
  else if (m_function == "linear") {
    if (sum < -1.0) return -1.0;
    else if (sum > 1.0) return 1.0;
    else return sum;
  }
  else {
    std::cout << "Neuron: Improperly assigned threshold function!" << std::endl;
  }
  return result;
}
