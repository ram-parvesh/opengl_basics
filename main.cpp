// Third-party library
#include <SDL2/SDL.h>

//C++ standard library
#include <iostream>



//globals
int gScreenheight = 640;
int gScreenWidth = 640;
SDL_Window* gGraphicApplicationWindow = nullptr;
SDL_GLContext gOpenGLcontext = nullptr; 


bool gQuit = false;  //if true quit window
 
void InitializeProgram(){
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"SDL_2 could not open vide subsystem"<<std::endl;
        exit(1);
    }


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    gGraphicApplicationWindow =  SDL_CreateWindow("OPENGL Window",
                                    0,0,
                                    gScreenWidth, gScreenheight,
                                    SDL_WINDOW_OPENGL);
    if(gGraphicApplicationWindow ==nullptr ){
        std::cout<< "SDL_window was not able to be created"<<std::endl;
        exit(1);
    }

    gOpenGLcontext = SDL_GL_CreateContext(gGraphicApplicationWindow);
    if (gOpenGLcontext == nullptr){
        std::cout<<"opengl context not available"<<std::endl;
        exit(1);
    }

}



void Input(){
    SDL_Event e;
    while(SDL_PollEvent(&e) !=0){
        if(e.type ==SDL_QUIT){
            std::cout<<"good Bye Ram"<<std::endl;
            gQuit=true;
        }
    }

}


void Predraw(){}
void Draw(){}


void Mainloop(){

    while(!gQuit){
        Input();
        Predraw();
        Draw();
        SDL_GL_SwapWindow(gGraphicApplicationWindow);
    }

}

void Cleanup(){
    SDL_DestroyWindow(gGraphicApplicationWindow);

    SDL_Quit();


}

int main(){

    InitializeProgram();

    Mainloop();

    Cleanup();

    return 0;

}