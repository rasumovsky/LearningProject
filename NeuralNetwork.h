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

#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "Axon.h"
#include "Neuron.h"

class NeuralNetwork 
{

 public:
  
  NeuralNetwork(int nInputs, int nOutputs, int nHiddenLayers, 
		int nNodesPerLayer);
  ~NeuralNetwork();
  
  // Accessors:
  std::vector<Neuron*> getBiasNodes();
  std::vector<Neuron*> getInputLayer();
  std::vector<Neuron*> getOutputLayer();
  std::vector<Neuron*> getLayer(int layerIndex);
  
  // Mutators:
  void addLayer(int layerIndex, int nodesPerLayer, string function);
  std::vector<double> getNetworkResponse(std::vector<double> vars);
  void randomizeNetworkWeights();
  void setNetworkTargets(std::vector<double> targets);
  void updateNetworkViaBP();

 private:
  
  int m_nInputs;
  int m_nOutputs;
  int m_nHiddenLayers;
  int m_nNodesPerLayer;
  
  // Keep pointers to all neurons and connections:
  std::vector<Axon*> m_axons;
  std::vector<Neuron*> m_neurons;
  
};

#endif
