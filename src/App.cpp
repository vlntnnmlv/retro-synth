#include <iostream>
#include <fstream>
#include <streambuf>

#include "logo.h"
#include "App.h"
#include "Utility.h"

App::App()
{
    m_pressed_keys = std::set<SDL_Keycode>();
    m_possible_keys = std::vector<SDL_Keycode>
    {
        SDLK_a,
        SDLK_w,
        SDLK_s,
        SDLK_e,
        SDLK_d,
        SDLK_f,
        SDLK_t,
        SDLK_g,
        SDLK_y,
        SDLK_h,
        SDLK_u,
        SDLK_j,
        SDLK_k
    };

    m_time = 0;

    m_gl_program_id = 0;
    m_gl_vertex_pos_2d_location = -1;
    m_gl_vao = 0;
    m_gl_vbo = 0;
    m_gl_ibo = 0;

    init();
}

App::~App()
{
    // Clean shaders
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDetachShader(m_gl_program_id, m_gl_vertex_shader);
    glDetachShader(m_gl_program_id, m_gl_fragment_shader);
    glDeleteProgram(m_gl_program_id);
    glDeleteShader(m_gl_vertex_shader);
    glDeleteShader(m_gl_fragment_shader);
    glDeleteTextures(1, &m_gl_tex);
    glDeleteBuffers(1, &m_gl_ibo);
    glDeleteBuffers(1, &m_gl_vbo);
    glDeleteVertexArrays(1, &m_gl_vao);
    SDL_GL_DeleteContext(m_gl_context);

    // Everything else
    SDL_DestroyWindow(m_window);
    SDL_CloseAudioDevice(m_audio_device_id);
    SDL_Quit();
}

int App::init()
{
    // SDL subsystems initialization
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Find and initialize audio engine
    init_audio();

    // Create application window
    init_window();
    init_video();

    return 0;
}

int App::init_audio()
{
    SDL_AudioSpec audio_spec_want, audio_spec;
    SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

    audio_spec_want.freq     = 44100;
    audio_spec_want.format   = AUDIO_F32;
    audio_spec_want.channels = 2;
    audio_spec_want.samples  = 512;

    audio_spec_want.callback = AudioEngine::callback;
    audio_spec_want.userdata = &m_audio_engine;

    m_audio_device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec_want, &audio_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if(!m_audio_device_id)
    {
        fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_PauseAudioDevice(m_audio_device_id, 0);

    return 0;
}

int App::init_window()
{
    m_window_width = 600;
    m_window_height = 600;
    m_window = SDL_CreateWindow(
            "Retro-Synth",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            m_window_width, m_window_height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

    if(!m_window)
    {
        fprintf(stderr, "Error creating SDL window. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    return 0;
}

int App::init_video()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_gl_context = SDL_GL_CreateContext(m_window);

    if (m_gl_context == NULL)
    {
        fprintf(stderr, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Use VSYNC
    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }

    // Initialize Shaders
    init_shaders();
    init_geometry();
    init_textures();

    return 0;
}

int App::init_shaders()
{
    GLint status;
    char err_buf[512];

    glGenVertexArrays(1, &m_gl_vao);
    glBindVertexArray(m_gl_vao);

    // Compile vertex shader
    m_gl_vertex_shader_content = read_file("shader/vertex.glsl");
    const char *vertex_shader_source = m_gl_vertex_shader_content->c_str();

    m_gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_gl_vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(m_gl_vertex_shader);
    glGetShaderiv(m_gl_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_vertex_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Vertex shader compilation failed: %s\n", err_buf);
        return 1;
    }

    // Compile fragment shader
    m_gl_fragment_shader_content = read_file("shader/fragment.glsl");
    const char *fragment_shader_source = m_gl_fragment_shader_content->c_str();

    m_gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_gl_fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(m_gl_fragment_shader);
    glGetShaderiv(m_gl_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_fragment_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Fragment shader compilation failed: %s\n", err_buf);
        return 1;
    }

    // Link vertex and fragment shaders
    m_gl_program_id = glCreateProgram();
    glAttachShader(m_gl_program_id, m_gl_vertex_shader);
    glAttachShader(m_gl_program_id, m_gl_fragment_shader);
    glBindFragDataLocation(m_gl_program_id, 0, "out_Color");
    glLinkProgram(m_gl_program_id);
    glUseProgram(m_gl_program_id);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 0;
}

int App::init_geometry()
{
    const GLfloat verts[4][4] = {
        //  x      y      s      t
        { -1.0f, -1.0f,  0.0f,  1.0f }, // BL
        { -1.0f,  1.0f,  0.0f,  0.0f }, // TL
        {  1.0f,  1.0f,  1.0f,  0.0f }, // TR
        {  1.0f, -1.0f,  1.0f,  1.0f }, // BR
    };

    // Populate vertex buffer
    glGenBuffers(1, &m_gl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    const GLint indicies[] = {
        0, 1, 2, 0, 2, 3
    };

    // Populate element buffer
    glGenBuffers(1, &m_gl_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // Bind vertex position attribute
    GLint pos_attr_loc = glGetAttribLocation(m_gl_program_id, "in_Position");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(pos_attr_loc);

    // Bind vertex texture coordinate attribute
    GLint tex_attr_loc = glGetAttribLocation(m_gl_program_id, "in_Texcoord");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_attr_loc);

    glUniform2f(glGetUniformLocation(m_gl_program_id, "in_Resolution"), m_window_width, m_window_height);
    glUniform1f(glGetUniformLocation(m_gl_program_id, "in_Time"), m_audio_engine.get_audio_time());
    glUniform1f(glGetUniformLocation(m_gl_program_id, "in_Amplitude"), m_audio_engine.get_amplitude());

    return 0;
}

int App::init_textures()
{
    glGenTextures(1, &m_gl_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gl_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glUniform1i(glGetUniformLocation(m_gl_program_id, "tex"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, logo_rgba);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void App::render()
{
    // Set shader uniforms
    glUniform1f(glGetUniformLocation(m_gl_program_id, "in_Time"), m_audio_engine.get_audio_time());
    glUniform1f(glGetUniformLocation(m_gl_program_id, "in_Amplitude"), m_audio_engine.get_amplitude());

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    SDL_GL_SwapWindow(m_window);
}

void App::start()
{
    m_running = true;
    while (m_running)
    {
        loop();
    }
}

void App::loop()
{
    Uint64 frame_start_time = SDL_GetPerformanceCounter();

    while(SDL_PollEvent(&m_current_event) != 0)
    {
        poll_event();
    }

    render();

    Uint64 frame_end_time = SDL_GetPerformanceCounter();
    m_delta_time = (frame_end_time - frame_start_time) / (float)SDL_GetPerformanceFrequency();
    m_time += m_delta_time;
}

void App::poll_event()
{
    if(m_current_event.type == SDL_QUIT)
        m_running = false;
    if (m_current_event.type == SDL_KEYDOWN)
    {
        if (m_current_event.key.keysym.sym == SDLK_ESCAPE)
            m_running = false;

        if (m_current_event.key.keysym.sym == SDLK_z)
        {
            m_audio_engine.decrease_octave();
        }

        if (m_current_event.key.keysym.sym == SDLK_x)
        {
            m_audio_engine.increase_octave();
        }

        auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
        if (it != m_possible_keys.end())
        {
            if (!m_pressed_keys.contains(m_current_event.key.keysym.sym))
            {
                m_pressed_keys.insert(m_current_event.key.keysym.sym);
                m_audio_engine.update_input(m_pressed_keys);
            }
        }
    }
    if (m_current_event.type == SDL_KEYUP)
    {
        auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
        if (it != m_possible_keys.end())
        {
            if (m_pressed_keys.contains(m_current_event.key.keysym.sym))
            {
                m_pressed_keys.erase(m_current_event.key.keysym.sym);
                m_audio_engine.update_input(m_pressed_keys);
            }
        }
    }   
}