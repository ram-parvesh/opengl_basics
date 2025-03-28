// for compilation code use the command
// g++ -std=c++17  Camera.cpp rog  ./src/glad.c -I./glm/ -I./include -lSDL2 -ldl

// Third-party library
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

//our libraries
#include "Camera.hpp"

//C++ standard library
#include <iostream>
#include <vector>
#include <string>
#include <fstream>


//globals
//screen Dimensions
int gScreenheight = 720;
int gScreenWidth = 1080;
SDL_Window* gGraphicApplicationWindow = nullptr;
SDL_GLContext gOpenGLcontext = nullptr; 

// main loop flag
bool gQuit = false;  //if true quit window

//VAO
GLuint VAO = 0;
//VBO
GLuint VBO = 0;
//IBO this is used to store the array of indices that we want to draw from,when we do index drawing
GLuint IBO = 0;

float uni_offset = -1.50f;
float unix_rotate = 0.0f;

//create a single global camera.
Camera gCamera;


//program object (for our shaders)
GLuint gGraphicsPipelineShaderProgram = 0;



std::string LoadShaderAsString(const std::string & filename){
    //resulting shader program loaded as a single string
    std::string result ="";

    std::string line ="";
    std::ifstream myFile(filename.c_str());

    if(myFile.is_open()){
        while(std::getline(myFile, line)){
            result+=line +"\n";
        }
        myFile.close();
    }

return result;

}
                               


GLuint CompileShader( GLuint type , const std::string & source){
    GLuint shaderObject;

    //based on the type passed on, we create shader object
    if(type==GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type==GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();

    //the source of our shader
    glShaderSource(shaderObject, 1, &src, nullptr);
    //now compile our shader
    glCompileShader(shaderObject);


    //retrieve the result of our compilation
    int result;

    //our goal with glGetShaderiv is to retrieve the compilation status
    glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&result);

    if(result ==GL_FALSE){
        int length;
        glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&length);
        char* errorMessages = new char[length]; // can also use allocation here
        glGetShaderInfoLog(shaderObject, length, &length,errorMessages);

        if(type ==GL_VERTEX_SHADER){
            std::cout<<"ERROR: GL_VERTEX_SHADER compilation failed. \n"<< errorMessages <<std::endl; 
        }
        else if(type==GL_FRAGMENT_SHADER){
            std::cout<<"ERROR: GL_FRAGMENT_SHADER compilation failed. \n"<< errorMessages <<std::endl;
        }
        // reclaim our memory
        delete[] errorMessages;

        //delete our broken shader
        glDeleteShader(shaderObject);
        }
    return shaderObject;
    // return 0;

}

GLuint CreateShaderProgram(const std::string& vertexshadersource, const std::string& fragmentshadersource ){

    //create a new program object
    GLuint programObject = glCreateProgram();
    
    //Compile our Shaders
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER,vertexshadersource);

    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER,fragmentshadersource);

    //one executable file.
    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject,myFragmentShader);
    glLinkProgram(programObject);

    //validate our program
    glValidateProgram(programObject);

    //once our final program object has been created, we can detach and then delete our individuals shaders
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    //delete the individual shaders once we are done
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline(){
    std::string gVertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string gFragmentShaderSource = LoadShaderAsString("./shaders/fragment.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource,gFragmentShaderSource);


}

void GetOpenGLVersionInfo(){

    std::cout<<"Vendor : "<< glGetString(GL_VENDOR) <<std::endl;
    std::cout<<"Rendered :"<< glGetString(GL_RENDERER) << std::endl;
    std::cout<<"Version :"<<glGetString(GL_VERSION) <<std::endl;
    std::cout<<"Shading Language Version : "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;

}

void VertexSpecification(){

    //lives on the CPU
    // const std::vector<GLfloat> vertexData
    //     {
        //     //0-vertex
        // -0.5f,-0.5f,0.0f,   //left vertex1
        // 1.0f,0.0f,0.0f,     // color

        //     //1-vertex
        // 0.5f,-0.5f,0.0f,   //right vertex2
        // 0.0f,1.0f,0.0f,     // color

        //     //2-vertex
        // -0.5f,0.5f,0.0f,    //top vertex3
        // 0.0f,0.0f,1.0f,     // color

        // //3-vertex 
        // 0.5f,0.5f,0.0f,   //left vertex1
        // 0.0f,0.0f,1.0f,     // color
        // };
        const std::vector<GLfloat> vertexData{
        1.0f,1.0f,1.0f, //vertex
        1.0f,0.0f,0.0f, //color
        0.0f,1.0f,1.0f, //vertex
        0.0f,1.0f,0.0f,//color
        1.0f,1.0f,0.0f,
        1.0f,0.0f,0.0f,//color
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f, //color
        1.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,//color
        0.0f,0.0f,1.0f,
        0.0f,1.0f,1.0f,//color
        0.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,//color
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,//color


        };
    
    // const std::vector<GLuint> indexBufferData {2,0,1,3,2,1};
    const std::vector<GLuint> indexBufferData {3,2,6,7,4,2,0,3,1,6,5,4,1,0};


    // vertex array object ----VAO
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO); 
    
    //start generating our VBO
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER ,VBO);


        //IndexedBufferObject
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indexBufferData.size()*sizeof(GLuint),
                indexBufferData.data(),
                GL_STATIC_DRAW);


    glBufferData(GL_ARRAY_BUFFER,
                vertexData.size()*sizeof(GL_FLOAT),
                vertexData.data(),
                GL_STATIC_DRAW);

    // setting up for vertex 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,       //type
                        GL_FALSE,       //Is the data normalized
                        sizeof(GL_FLOAT)*6,              //Stride
                        (void*)0        //Offset
                        );

    //setting up for color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,       //type
                        GL_FALSE,       //Is the data normalized
                        sizeof(GL_FLOAT)*6,              //Stride
                        (GLvoid*)(sizeof(GL_FLOAT)*3)        //Offset
                        );

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


}


void InitializeProgram(){
    // initilize SDL
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"SDL_2 could not open vide subsystem"<<std::endl;
        exit(1);
    }

    // setup the openGL context
    //use OpenGL 4.1 core or greater
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    //request a double buffer for smooth updateing.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // create an application window using OpenGK that suppports SDL
    gGraphicApplicationWindow =  SDL_CreateWindow("OPENGL Window",
                                    0,0,
                                    gScreenWidth, gScreenheight,
                                    SDL_WINDOW_OPENGL);

    // check if the window did not create. 
    if(gGraphicApplicationWindow ==nullptr ){
        std::cout<< "SDL_window was not able to be created"<<std::endl;
        exit(1);
    }

    // create an OpenGL Graphics Context
    gOpenGLcontext = SDL_GL_CreateContext(gGraphicApplicationWindow);
    
    if (gOpenGLcontext == nullptr){
        std::cout<<"opengl context not available"<<std::endl;
        exit(1);
    }


    // initialide the GLAD library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        std::cout<<"glad was not initialized"<<std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
    

}




void Input(){
    //event handlers that handles various events in SDL
    SDL_Event e;
    static int mouseX =gScreenWidth/2;  
    static int mouseY = gScreenheight/2;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //handle evnets on queue
    while(SDL_PollEvent(&e) !=0){
        // if(e.type ==SDL_QUIT)
        
        if(state[SDL_SCANCODE_ESCAPE]){
            std::cout<<"good Bye "<<std::endl;
            gQuit=true;
        }
        else if(e.type ==SDL_MOUSEMOTION){
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            gCamera.MouseLook(mouseX,mouseY);

        }
    }
    float speed = 0.1f;
    unix_rotate+=0.1f;
    std::cout<<"Offset :"<< unix_rotate<<std::endl;
    
    if (state[SDL_SCANCODE_UP] ){
        gCamera.MoveForward(speed);

        // uni_offset+=0.1f;
        // std::cout<<"Offset :"<< uni_offset<<std::endl;

    }

    if (state[SDL_SCANCODE_DOWN] ){
        gCamera.MoveBackward(speed);
        // uni_offset-=0.1f;
        // std::cout<<"Offset :"<< uni_offset <<std::endl;

    }


        if (state[SDL_SCANCODE_RIGHT] ){
            gCamera.MoveRight(speed);
        // unix_rotate+=0.1f;
        // std::cout<<"Offset :"<< unix_rotate<<std::endl;

    }
        if (state[SDL_SCANCODE_LEFT] ){
            gCamera.MoveLeft(speed);
        // unix_rotate-=0.1f;
        // std::cout<<"Offset :"<< unix_rotate<<std::endl;

    }


}


void Predraw(){
    //disable depth test and face culling.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //initilize clear color
    //this is the background of the screeen
    glViewport(0,0,gScreenWidth,gScreenheight);
    glClearColor(1.f, 1.f, 0.f,1.f);


    //clean color buffer and depth buffer
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //use our shader
    glUseProgram(gGraphicsPipelineShaderProgram);

    // unix_rotate+=0.1f;
    // std::cout<<"Offset :"<< unix_rotate<<std::endl;

    //Model transformation by translating our object into world space
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,uni_offset));
    //update our model matrix by applying a rotation after our translation 
    model =  glm::rotate(model,glm::radians(unix_rotate),glm::vec3(0.0f,1.0f,0.0f));
    //update our model matrix by applying a scaling after rotation  
    model =  glm::scale(model,glm::vec3(1.0f,1.0f,1.0f));

    GLint modelmatrixlocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "ModelMatrix");
    // std::cout<<"location of u_Offset :"<<location<<std::endl;

    if(modelmatrixlocation >=0){
             glUniformMatrix4fv(modelmatrixlocation, 1,GL_FALSE, &model[0][0] );
    }
    else{
        std::cout<<"could not find modelmatrix"<<std::endl;
        exit(EXIT_FAILURE);
    }

    glm::mat4 view = gCamera.GetViewMatrix();
    GLint ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "ViewMatrix");
    if(ViewLocation >=0){
            glUniformMatrix4fv(ViewLocation, 1,GL_FALSE, &view[0][0] );
   }
   else{
       std::cout<<"could not find ViewMatrix"<<std::endl;
       // exit(EXIT_FAILURE);
   }



    //projection matrix (in perspective)
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
                                            (float)gScreenWidth/(float)gScreenheight,
                                            0.1f,
                                            10.0f);
                        
    GLint perspectivelocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "Perspective");
     if(perspectivelocation >=0){
             glUniformMatrix4fv(perspectivelocation, 1,GL_FALSE, &perspective[0][0] );
    }
    else{
        std::cout<<"could not find perspectivematrix"<<std::endl;
        // exit(EXIT_FAILURE);
    }
    





}

//Draw function
//the render function gets called once per loop
void Draw(){

    //enable our attributes
    glBindVertexArray(VAO);

    //select the vertex buffer object we want to enable
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //render data
    // glDrawArrays(GL_TRIANGLES,0,6);    // draw command using VAO

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0); //using index draw command
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT,0); 
    //Stop using our current graphics pipeline
    //Note:this is not necessary if we only have one graphics pipeline
    glUseProgram(0);

}


void Mainloop(){
    SDL_WarpMouseInWindow(gGraphicApplicationWindow,gScreenWidth/2,gScreenheight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    while(!gQuit){
        //handle Input
        Input();
        // setup anything (ie OpenGL State) that needs to take 
        // place before draw calls
        Predraw();
        // draw Calls in OpenGL
        Draw();
        //update screen of our Specified window
        SDL_GL_SwapWindow(gGraphicApplicationWindow);
    }

}

void CleanUp(){
    SDL_DestroyWindow(gGraphicApplicationWindow);

    SDL_Quit();


}

int main(){

    // 1.set up the graphics program
    InitializeProgram();

    // 2.setup our geometry
    VertexSpecification();
    
    // 3.create our graphics pipeline
    // - at a minimum , this means the vertex and fragment shader
    CreateGraphicsPipeline();

    // 4. call the main application loop
    Mainloop();

    // 5.call the cleanup function when our program terminates
    CleanUp();

    return 0;

}