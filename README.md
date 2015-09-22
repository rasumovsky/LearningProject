# LearningProject
  This package is just a playground for learning about neural networks. The idea
  is to make a feed-forward back-propagational neural network. The test case 
  will be separation of gluon-fusion produced Higgs to gamma gamma signals 
  from non-resonant diphoton backgrounds.

## Axon
   A basic class for representing a directed weighted connection between two 
   nodes.

## Neuron
   A basic class for representing network nodes. This includes the activation
   function, the incoming connections, and the outgoing connections. There is 
   also the option to create the node as a bias node, with a constant response
   of 1.0. 

## NeuralNetwork
   A class for creating a network with a given number of variables, a given 
   number of hidden layers, and a given number of nodes per hidden layer. The 
   first (visible) layer should have a linear activation function, and the input
   variables should be transformed to the interval [-1,+1], preferrably in a
   uniform manner (whatever that means). The output layer(s) should also have
   linear activation functions, so that the output is on the interval [-1,+1].
   
   The response of the network can be found simply by calling the getResponse()
   method of the output layer node(s). This recursively calls the same method
   for upstream neurons, if the output has not already been calculated following
   the most recent call to the clearResponse() method. 