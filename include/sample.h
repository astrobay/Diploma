#ifndef SAMPLE_H
#define SAMPLE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
class Sample
{
  public:
    double theta;
    double phi;
    vec3 dir;
    double * shValues;
    Sample()	:	shValues(NULL) {}
    ~Sample()
  	{
  		if(shValues)
  			delete [] shValues;
  		  shValues=NULL;
  	}
};
#endif
