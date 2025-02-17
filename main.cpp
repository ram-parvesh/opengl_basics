// for compilation code use the command
// g++ main.cpp -o prog  ./src/glad.c -I./include -lSDL2 -ldl 

// Third-party library
#include <SDL2/SDL.h>
#include <glad/glad.h>

//C++ standard library
#include <iostream>
#include <vector>
#include <string>
#include <fstream>


//globals
//screen Dimensions
int gScreenheight = 640;
int gScreenWidth = 640;
SDL_Window* gGraphicApplicationWindow = nullptr;
SDL_GLContext gOpenGLcontext = nullptr; 

// main loop flag
bool gQuit = false;  //if true quit window

//VAO
GLuint gVertexArrayObject = 0;
//VBO
GLuint gVertexBufferObject =0;

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
    if(type==GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type==GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();

    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);


    return shaderObject;

}

GLuint CreateShaderProgram(const std::string& vertexshadersource, const std::string& fragmentshadersource ){

    GLuint programObject = glCreateProgram();
    
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER,vertexshadersource);

    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER,fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject,myFragmentShader);

    glLinkProgram(programObject);

    //validate our program
    glValidateProgram(programObject);

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
    const std::vector<GLfloat> vertexPosition{
        -0.8f,-0.8f,0.0f,   //vertex1
        0.8f,-0.8f,0.0f,   //vertex2
        0.0f,0.8f,0.0f    //vertex 2

    };
    
    glGenVertexArrays(1,&gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject); 
    
    //start generating our VBO
    glGenBuffers(1,&gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER ,gVertexBufferObject);

    glBufferData(GL_ARRAY_BUFFER,
                vertexPosition.size()*sizeof(GLfloat),
                vertexPosition.data(),
                GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void*)0
                        );

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

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
    SDL_Event e;
    while(SDL_PollEvent(&e) !=0){
        if(e.type ==SDL_QUIT){
            std::cout<<"good Bye "<<std::endl;
            gQuit=true;
        }
    }

}


void Predraw(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0,0,gScreenWidth,gScreenheight);
    glClearColor(1.f, 1.f, 0.1f,1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}


void Draw(){

    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glDrawArrays(GL_TRIANGLES,0,3);

}


void Mainloop(){

    while(!gQuit){
        Input();
        Predraw();
        Draw();
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