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
  setWeight(weight);
  setOriginNeuron(originNeuron);
  setTerminalNeuron(terminalNeuron);
  return;
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
   @returns - The weight of the connection.
*/
double Axon::getWeight() {
  return m_weight;
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
