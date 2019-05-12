#include "include/initGL.hpp"
#include <cmath>
//***********************
#include "include/sh.h"
#include "include/sample.h"
#include "include/genSamples.h"

struct structSample{
    vec4 dir;
    float shValues[16];
};

void copy_from_class_to_struct(structSample *structSamples, Sample *samples, int numSamples)
{
  for (int i=0; i < numSamples; i++) {
    structSamples[i].dir=vec4(samples[i].dir,0.0f);
    for (int j=0; j<16; j++) {
      structSamples[i].shValues[j]=samples[i].shValues[j];
    }
  }
}

int main()
{
  GLFWwindow* window;
  if (init_GL(window)) {
    cout<<"ERROR IN GL INITIALIZATION!"<<" \n";
    return -1;
  }


  Shader shader("shaders/vertex", "shaders/fragment");
  Shader skyboxShader("shaders/skyvertex", "shaders/skyfragment");
  // cube VAO
  unsigned int cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  //bind
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  //unbind
  glBindVertexArray(0);
  // skybox VAO
  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  //bind
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(cubeVAO);

  //unsigned int cubeTexture = loadTexture("textures/marble.jpg");
  unsigned int cubemapTexture = loadCubemap(faces);

  shader.use();
  shader.setInt("texture1",0);
  skyboxShader.use();
  skyboxShader.setInt("skybox",0);
//*****************************************************************************
//Generate numSamples Samples
  const int sqrtNumSamples=100;
  const int numSamples=sqrtNumSamples*sqrtNumSamples;
  const int numBands=4;
  const int numFunctions=numBands*numBands;
  const int resSize=numFunctions*numSamples;
  vec4 result[resSize];
  Sample *samples=NULL;
  samples=new Sample[numSamples];
  structSample structSamples[numSamples];
  GenerateSamples(sqrtNumSamples, numBands, samples);
  copy_from_class_to_struct(structSamples,samples,numSamples);
  for (int i=0; i<resSize; i++) {
    result[i].x=5;
    result[i].y=5;
    result[i].z=5;
    result[i].w=5;
  }
  /*for (int i=0; i<numSamples; i++) {
    cout << " class dir: " << samples[i].dir.x << samples[i].dir.y
      << samples[i].dir.z << " str dir: " << structSamples[i].dir.x
      << structSamples[i].dir.y << structSamples[i].dir.z << " "<<structSamples[i].dir.a << "\n";
      cout << "\n";
    for (int j=0; j<16; j++) {
      cout << " classval"<<samples[i].shValues[j] << " structval " << structSamples[i].shValues[j] << "\n";
    }
      cout << "\n";
  } */
/*  for (int i=0; i<numSamples; i++) {
    cout << structSamples[i].dir.x << " "<< structSamples[i].dir.y << " " << structSamples[i].dir.z << "\n";
    cout << structSamples[i].shValues[1] << "\n";
  }
  cout << "\n"; */
/*  for (int i=0; i<numFunctions; i++) {
    cout << structSamples[0].shValues[i] << " ";
  }
  cout << "\n";
  cout << "\n"; */
//Create compute shader
  Shader compute(NULL,NULL,"shaders/computeTest");
  compute.use();
  compute.setInt("skybox",0);
  unsigned int ssboInput, ssboRes;
  glGenBuffers(1, &ssboInput);
  glGenBuffers(1, &ssboRes);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssboInput);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(structSamples), &structSamples[0], GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,ssboInput);
  //*************
  glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssboRes);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(result), result, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER,1,ssboRes);

  glDispatchCompute(10,1,1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  vec4 *res;
  vec4 koeffs[numFunctions];
  for (int i=0; i<numFunctions; i++) {
    koeffs[i].x=0;
    koeffs[i].y=0;
    koeffs[i].z=0;
    koeffs[i].w=0;
  }
  res = (vec4 *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

  for (int i=0; i<numSamples; i++) {
    for (int j=0; j<numFunctions; j++) {
      koeffs[j]+=res[i*numFunctions+j];
      cout << res[i*numFunctions+j].x << " | " << res[i*numFunctions+j].y << " | " << res[i*numFunctions+j].z << "\n";
    }
    cout << "\n";
  }
  float weight=4*M_PI/numSamples;
  for (int i=0; i<numFunctions; i++) {
    koeffs[i]*=weight;
  }
    cout << "\n";
      cout << "\n";
  for (int i=0; i<numFunctions; i++) {
    cout << koeffs[i].x << " | " << koeffs[i].y << " | " << koeffs[i].z << "\n";
  }
  cout << "\n";
  /*structSample *in;
  in = (structSample *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  for (int i=0; i<numFunctions; i++) {
    cout << in[0].shValues[i] << " ";
  }
  cout << "\n";
  cout << "\n";
  for (int i=0; i<numFunctions; i++) {
    cout << res[i].x << " ";
  }
  cout << "\n"; */
/*  for (int i=0; i<numFunctions; i++) {
    cout << p[i].x << " "<< p[i].y << " "<<p[i].z << "\n";
    //cout << p[i].shValues[1] << "\n";
  } */
//*****************************************************************************

  while (!glfwWindowShouldClose(window))
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      processInput(window,camera,deltaTime);
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // draw scene as normal
      shader.use();
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", projection);
      shader.setVec3("cameraPos", camera.Position);

      glBindVertexArray(cubeVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      // draw skybox as last
      glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
      skyboxShader.use();
      view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
      skyboxShader.setMat4("view", view);
      skyboxShader.setMat4("projection", projection);
        // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth function back to default

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);

    glfwTerminate();

    return 0;
}
