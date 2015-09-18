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
   @param function - The activation function for the neuron: "logistic", "tanh"
 */
Neuron::Neuron(std::string function) {
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
  m_upstreamConnections->push_back(axon);
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
   @returns - The neuron response.
*/
double Neuron::getResponse() {
  double response = 0.0;
  // Loop over input connections
  for (std::vector<Axon*>::iterator axonIter = m_upstreamConnections.begin();
       axonIter != m_upstreamConnections.end(); axonIter++) {
    // sum the input weights:
    response += axonIter->getWeight();
  }
  // Then get the result of the threshold function:
  return thresholdFunction(response);
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
   Evaluate the threshold function to see whether the neuron should fire.
   @param sum - The sum of weights of the input connections.
   @returns - The response for a given sum and function.
*/
double Neuron::thresholdFunction(double sum) {
  double result = 0;
  if (m_function=="logistic") {
    result = 1.0 / (1 + exp(-1.0*sum));
  }
  else if(m_function=="tanh") {
    result = (exp(sum) - exp(-1.0*sum)) / (exp(sum) + exp(-1.0*sum));
  }
  else {
    std::cout << "Neuron: Improperly assigned threshold function!" << std::endl;
  }
  return result;
}
