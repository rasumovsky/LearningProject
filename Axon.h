////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: Axon.h                                                              //
//  Class: Axon.cxx                                                           //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef Axon_h
#define Axon_h

#include <stdlib.h>
#include <stdio.h>
#include "Neuron.h"

class Axon {
  
 public:
  
  Axon(double weight, Neuron *originNeuron, Neuron *terminalNeuron);
  ~Axon();
  
  // Public Accessors:
  double getLearningRate();
  Neuron* getOriginNeuron();
  Neuron* getTerminalNeuron();
  double getWeight();
  
  // Public Mutators:
  void setLearningRate(double alpha);
  void setOriginNeuron(Neuron *neuron);
  void setTerminalNeuron(Neuron *neuron);
  void setWeight(double weight);
  double trainWeight();
  
 private:
  
  // Member objects:
  double m_rate;
  double m_weight;
  Neuron *m_originNeuron;
  Neuron *m_terminalNeuron;
  
};

#endif
