////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: NeuralNetwork.h                                                     //
//  Class: NeuralNetwork.cxx                                                  //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 06/03/2015                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef NeuralNetwork_h
#define NeuralNetwork_h

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "Axon.h"
#include "Neuron.h"

class NeuralNetwork 
{

 public:
  
  NeuralNetwork();
  ~NeuralNetwork();
  
  // Mutators:
  void addNewLayer(int nodesPerLayer);
  void addSingleNode(int layer, Neuron *newNeuron);
  void train();
  
  // Accessors:
  std::vector<Neuron> getLayer();
  std::vector<Neuron> getNetwork();
  double getResponse();
  
 private:
  
  void backPropagate();
  
};

#endif
