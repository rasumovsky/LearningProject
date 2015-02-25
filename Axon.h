////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Axon.h                                                              //
//  Class: Axon.cxx                                                           //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 24/02/2015                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef Axon_h
#define Axon_h

#include <stdlib.h>
#include <stdio.h>
#include "Neuron.h"

class Axon 
{
  
 public:
  
  Axon(double newWeight, Neuron *newNeuron);
  ~Axon();
  
  // Mutators:
  void setWeight(double newWeight);
  void setNeuron(Neuron newNeuron);
  
  // Accessors:
  double getWeight();
  Neuron* getNeuron();
  
 private:
  
  // Member objects:
  double weight;
  Neuron *neuron;
  
};

#endif
