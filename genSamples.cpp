#include <iostream>
#include "include/genSamples.h"
#include "include/sh.h"


bool GenerateSamples(int sqrtNumSamples, int numBands, Sample * samples)
{
  srand(time(NULL));
  int numSamples=sqrtNumSamples*sqrtNumSamples;
  int numFunctions=numBands*numBands;
  int index=0;
  for(int i=0; i<numSamples; ++i)
  {
    samples[i].shValues=new double[numFunctions];
    if(!samples[i].shValues)
    {
      std::cout<< "Unable to allocate space for SH values in samples";
      return false;
    }
  }

  for (int i=0; i<sqrtNumSamples; ++i) {
    for (int j=0; j<sqrtNumSamples; ++j) {
      //Generate the position of this sample in [0, 1)x[0, 1)
      double x=(i+((double)rand()/RAND_MAX))/sqrtNumSamples;
      double y=(j+((double)rand()/RAND_MAX))/sqrtNumSamples;

      double theta=2.0*acos(sqrt(1.0-x));
      double phi=2.0*M_PI*y;
      samples[index].theta=theta;
      samples[index].phi=phi;
      samples[index].dir=vec3(float(sin(theta)*cos(phi)),
        float(sin(theta)*sin(phi)),float(cos(theta)));

      for(int l=0; l<numBands; ++l) {
        for(int m=-l; m<=l; ++m) {
            int index2=l*(l+1)+m;
            samples[index].shValues[index2]=SH(l,m,theta, phi);
        }
      }
      ++index;
    }
  }
  return true;
}
