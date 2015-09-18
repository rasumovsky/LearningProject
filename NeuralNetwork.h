////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  Name: NeuralNetwork.h                                                     //
//  Class: NeuralNetwork.cxx                                                  //
//                                                                            //
//  Author: Andrew Hard                                                       //
//  Email: ahard@cern.ch                                                      //
//  Date: 18/09/2015                                                          //
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
  
  NeuralNetwork(int nInputs, int nHiddenLayers, int nNodesPerLayer);
  ~NeuralNetwork();
  
  // Mutators:
  void addLayer(int nodesPerLayer);
  
  // Accessors:
  std::vector<Neuron*> getLayer(int layerIndex);
  double getNetworkResponse(std::vector<double> inputs);
  
 private:
  
  int m_nInputs;
  int m_nHiddenLayers;
  int m_nNodesPerLayer;
  
  // Keep pointers to all neurons and connections:
  std::vector<Axon*> m_axons;
  std::vector<Neuron*> m_neurons;
  
};

#endif
