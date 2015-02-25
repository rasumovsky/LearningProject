////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Neuron.cxx                                                          //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 24/02/2015                                                          //
//                                                                            //
//  This class represents a neuron in a neural network. The connections       //
//  are to upstream ancestor nodes that fire before the given node.           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Neuron.h"

/**
   Neuron constructor, with option to set the threshold function:
 */
Neuron::Neuron(std::vector<*Axon> *newConnections, std::string newFunction) {
  randomBit = false;
  responseBit = false;
  response = 0.0;
  connections = newConnections;
  function = newFunction;
  return;
}

/**
   Neuron constructor.
 */
Neuron::Neuron(std::vector<*Axon> *newConnections) {
  Neuron(newConnections, "logistic");
  return;
}

/**
   Neuron default constructor.
*/
Neuron::Neuron() {
  std::vector<*Axon> newConnections = new std::vector<*Axon>();
  newConnections->clear();
  Neuron(newConnections, "logistic");
  return;
}

/**
   Add a connection to another neuron.
*/
void Neuron::addConnection(Axon *newAxon) {
  connections->push_back(newAxon);
}

/**
   Sets the random and response bits false for this Neuron and all upstream
   Neurons as well. This could fail for feedback loops...
 */
void Neuron::clearBits() {
  randomBit = false;
  responseBit = false;
  for (int i_a = 0; i_a < (int)connections->size(); i_a++) {
    connections[i_a]->getNeuron()->clearBits();
  }
}

/**
   Randomizes weights feeding into this node only.
*/
void randomizeWeights() {
  for (int i_a = 0; i_a < (int)connections->size(); i_a++) {
    double randomWeight = ((((double)(rand() % 20)) - 10.0) / 10.0);
    connections[i_a]->setWeight(randomWeight);
    if (!connections[i_a]->getNeuron()->isRandomized()) {
      connections[i_a]->getNeuron()->randomizeWeights();
    }
  }
  randomBit = true;
}

/**
   Clear the list of connected neurons.
*/
void Neuron::removeAllConnections() {
  connections->clear();
}

/**
   Remove a given pointer to a neuron from the connections.
 */
void Neuron::removeConnection(Axon *oldAxon) {
  int pos = -1;
  for (int i_a = 0; i_a < (int)connections->size(); i_a++) {
    if (connections[i_a] == oldAxon) {
      pos = i_a;
      break;
    }
  }
  if (pos != -1) {
    connections->erase(connections.begin()+pos);
  }
}

/**
   Resets the Neuron response.
*/ 
void Neuron::setResponse(double newResponse) {
  response = newResponse;
}

/**
   Get the response of the Neuron to its inputs, recursively by getting 
   the responses for all previous neurons as well.
*/
double Neuron::evaluateResponse() {
  double weightSum = 0.0;
  // loop over connections, add weighted responses:
  for (int i_a = 0; i_a < (int)connections->size(); i_a++) {
    
    // if ancestor node's response has already been evaluated, no need to call
    // this method recursively for ancestor nodes:
    if (connections[i_a]->getNeuron()->hasResponse()) {
      weightSum += (connections[i_a]->getNeuron()->getStaticResponse() *
		    connections[i_a]->->getWeight());
    }
    // ancestor node's response has not already been evaluated. call this
    // method recursively. 
    else {
      weightSum += (connections[i_a]->getNeuron()->evaluateResponse() *
		    connections[i_a]->->getWeight());
    }
  }
  
  response = thresholdFunction(weightSum);
  return response;
}

/**
   Returns the number of predecessor connections for the Neuron.
*/
int Neuron::getNConnections() {
  if (connections == NULL) {
    return 0;
  }
  else {
    return (int)connections->size();
  }
}

/**
   Simply checks the activated bool object. Does not propagate the checking
   throughout the entire network structure like isActivated(). This method is 
   much faster for simple status checks.
 */
double Neuron::getStaticResponse() {
  return response;
}

/**
   Returns whether the response has been updated in the current iteration.
 */
bool Neuron::hasResponse() {
  return responseBit;
}

/**
   Returns whether the response has been randomized in this iteration.
 */
bool Neuron::isRandom() {
  return randomBit;
}

/**
   Evaluate the threshold function to see whether the neuron should fire.
 */
double Neuron::thresholdFunction(double sum) {
  double result = 0;
  if (function=="logistic") {
    result = 1.0 / (1 + exp(-1.0*sum));
  }
  else if(function=="tanh") {
    result = (exp(sum) - exp(-1.0*sum)) / (exp(sum) + exp(-1.0*sum));
  }
  else {
    std::cout << "Neuron: Improperly assigned threshold function!" << std::endl;
  }
  return result;
}
