#include <random>
#include <fstream>

#include "hopfield.h"



using namespace std;

Hopfield::Hopfield(int size){
  weights.resize(size, vector<float>(size, 0));
  state.resize(size);
  bias.resize(size);
  this->size = size;
  for(int row = 0; row < size; row++){
    double randProb = (double)  rand()/RAND_MAX;
    if(randProb < 0.5)	state[row] = 1;
    else state[row] = -1;
    
    randProb = (double) rand()/RAND_MAX;
    double randBias = (double) rand()/RAND_MAX;
    
    if(randProb < 0.5) bias[row] = -1* randBias;
    else bias[row] = randBias;

    for(int col = row; col < size; col++){
      if(row == col) weights[row][col] = 0;
      else{
	randProb = (double) rand()/RAND_MAX;
	double randWeight = (double) rand()/RAND_MAX;
	
	if(randProb < 0.5) {
	  weights[row][col] = -1*randWeight;
	  weights[col][row] = -1*randWeight;
	}
	else {
	  weights[row][col] = randWeight;
	  weights[col][row] = randWeight;
	}
      }
    }      
  }
}

float Hopfield::getStim(int neuron){
  int row = neuron;
  float totalWeight = 0.0f;
  for(int col = 0; col < size; col++){
    if(row != col){
      totalWeight += weights[row][col] * state[col];
    }
  }
  return totalWeight;
}

float Hopfield::getEnergy(){
  float weightsEnergy = 0;
  float biasEnergy = 0;
  for(int row = 0; row < size; row++){
    biasEnergy += bias[row]*state[row];
    for(int col = 0; col < size; col++){
      weightsEnergy += weights[row][col]*state[row]*state[col];
    }
  }
  return -.5*weightsEnergy + biasEnergy;
}

void Hopfield::update(int steps){
  for(int i = 0; i < steps; i++){
    int randNeuron = rand()%size;
    //cout << "attempt to flip neuron #"  << randNeuron << endl;
    float stim = getStim(randNeuron);
    if(stim > bias[randNeuron]){
      state[randNeuron] = 1;
    }
    
    else if (stim < bias[randNeuron]){
      state[randNeuron] = -1;
    }

    //cout << getEnergy() << endl;
  }
}

/**
 * WE ASSUME THAT THE WEIGHTS MATRIX IS ALREADY CORRECTLY SIZED
 */
void Hopfield::trainWeights(vector<string> bitstrings){
  float size = bitstrings.size();
  for(string str : bitstrings){
    for(int row = 0; row < str.length(); row++){
      if(str[row] == '0'){
        for(int col = 0; col < str.length(); col++){
	  weights[row][col] = -1*(str[col] - '0');
	}
      }
      else{
	for(int col = 0; col < str.length(); col++){
	  int weightVal = str[col] - '0';
	  weights[row][col] = (float)weightVal/size;
	  if(col == row) weights[row][col] = 0;
	}
      }
    }
  }
}

void Hopfield::setState(string instate){
  for(int i = 0; i < instate.size(); i++){
    if(instate[i] = '1') state[i] = 1;
    else state[i] = -1;
  }
}

void Hopfield::setState(vector<int> & instate){
  for(int i = 0; i < state.size(); i++){
    state[i] = instate[i];
  }
}

void Hopfield::corrupt(int numstates){
  for(int i = 0; i < numstates; i++){
    int randInt = rand()%2;
    if(randInt == 1) state[i]*=-1;
  }
}

void Hopfield::corruptRandom(int numstates){
  vector<int> remaining;
  remaining.resize(state.size(), 0);
  for(int i = 0; i < state.size(); i++){
    remaining[i] = i;
  }

  for(int i = 0; i < numstates; i++){
    int randIdx = rand()%remaining.size();
    //cout << "flipping the neuron in: " <<  remaining[randIdx] << endl;
    state[remaining[randIdx]] *= -1;
    remaining.erase(remaining.begin() + randIdx);    
  }
  
}

float Hopfield::hamming(vector<int> otherState){
  int dist = 0;
  for(int i = 0; i < state.size(); i++){
    if(state[i] != otherState[i]) dist++;
  }
  return dist;
}

void Hopfield::randomize(){
  
}

void Hopfield::printConfiguration(){
  /*
  cout << "WEIGHTS============================" << endl;
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      cout << weights[row][col] << " ";
    }
    cout << endl;
  }
  */
  cout << "STATE==============================" << endl;
  for(auto e: state) cout << e << " ";
  cout << endl;

  //for(auto e: bias) cout << e << " ";
  //cout << endl;
}

void Hopfield::writeArrToFile(string filename, vector<float> myvec){
  ofstream myfile;
  myfile.open(filename);
  for(int i = 0; i < myvec.size(); i++){
    myfile << myvec[i] << endl;
  }
  myfile.close();
  
}

vector<int> Hopfield::toStateVector(string strstate){
  vector<int> ret;
  for(int idx = 0; idx < strstate.length(); idx++){
    if(strstate[idx] == '0') ret.push_back(-1);
    else ret.push_back(1);
  }
  return ret;
}
