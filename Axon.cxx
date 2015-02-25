////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Axon.cxx                                                            //
//                                                                            //
//  Created: Andrew Hard                                                      //
//  Email: ahard@cern.ch                                                      //
//  Date: 24/02/2015                                                          //
//                                                                            //
//  This class represents a connection from one neuron to another.            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "Axon.h"

/**
   Axon constructor.
 */
Axon::Axon(double newWeight, Neuron *newNeuron) {
  weight = newWeight;
  neuron = newNeuron;
  return;
}

/**
   Set the weight of the connection.
*/
void Axon::setWeight(double newWeight) {
  weight = newWeight;
}

/**
   Set the connected neuron.
*/
void Axon::setNeuron(Neuron *newNeuron) {
  neuron = newNeuron;
}

/**
   Get the weight of the connection.
*/
double Axon::getWeight() {
  return weight;
}

/**
   Get a pointer to the connected neuron.
*/
Neuron* Axon::getNeuron() {
  return neuron;
}
