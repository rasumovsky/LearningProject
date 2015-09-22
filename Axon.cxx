////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Axon.cxx                                                            //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
//  This class represents a weighted connection from one neuron to another.   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Axon.h"

/**
   -----------------------------------------------------------------------------
   Axon constructor. Creates a connection between two nodes with a weight.
   @param weight - The new connection weight.
   @param neuron - The upstream neuron for the connection.
   @param neuron - The downstream neuron for the connection.
 */
Axon::Axon(double weight, Neuron *originNeuron, Neuron *terminalNeuron) {
  setLearningRate(0.2);
  setOriginNeuron(originNeuron);
  setTerminalNeuron(terminalNeuron);
  setWeight(weight);
  return;
}

/**
   -----------------------------------------------------------------------------
   Get the learning rate (the rate at which the gradient descent will be 
   pursued.
   @returns - The learning rate.
*/
double Axon::getLearningRate() {
  return m_rate;
}
/**
   -----------------------------------------------------------------------------
   @returns - A pointer to the upstream connected neuron.
*/
Neuron* Axon::getOriginNeuron() {
  return m_originNeuron;
}

/**
   -----------------------------------------------------------------------------
   @returns - A pointer to the downstream connected neuron.
*/
Neuron* Axon::getTerminalNeuron() {
  return m_terminalNeuron;
}

/**
   -----------------------------------------------------------------------------
   @returns - The weight of the connection.
*/
double Axon::getWeight() {
  return m_weight;
}

/**
   -----------------------------------------------------------------------------
   Set the learning rate (the rate at which the gradient descent will be 
   pursued.
   @param rate - The new learning rate.
*/
void Axon::setLearningRate(double rate) {
  m_rate = rate;
}

/**
   -----------------------------------------------------------------------------
   Set the upstream neuron.
   @param neuron - The upstream neuron for the connection.
*/
void Axon::setOriginNeuron(Neuron *neuron) {
  if (!neuron) {
    std::cout << "Axon: ERROR! origin neuron is null." << std::endl;
    exit(0);
  }
  m_originNeuron = neuron;
}

/**
   -----------------------------------------------------------------------------
   Set the downstream neuron.
   @param neuron - The downstream neuron for the connection.
*/
void Axon::setTerminalNeuron(Neuron *neuron) {
  if (!neuron) {
    std::cout << "Axon: ERROR! origin neuron is null." << std::endl;
    exit(0);
  }
  m_terminalNeuron = neuron;
}

/**
   -----------------------------------------------------------------------------
   Set the weight of the connection.
   @param weight - The new connection weight.
*/
void Axon::setWeight(double weight) {
  m_weight = weight;
}

/**
   -----------------------------------------------------------------------------
   Change the weight based on training.
*/
double Axon::trainWeight() {
  // NEED TO THINK ABOUT THIS
  double o_i = m_originNeuron->getResponse();//o_i
  double delta_j = m_terminalNeuron->getDelta();//delta_j
  double deltaW_ij = -1.0 * m_rate * delta_j * o_i;
  m_weight += deltaW_ij;
}
