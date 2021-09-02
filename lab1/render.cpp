//
//  render.cpp
//  lab1
//
//  Created by Sam Xie on 1/2/18.
//  Copyright © 2018 University of Singapore. All rights reserved.
//

#include "render.hpp"

#define Y_OFFSET -2 // -2 for Triangle | 0 for full screen

#define NUM_PARTICLES 200
#define PARTICLE_VICINITY 2
#define PARTICLE_SPEED 1
#define PARTICLE_DRAG 5 // MODE1: 0 | MODE2-explosion: 5
#define PARTICLE_SIZE 0.05f // MODE1: 0.1f | MODE2: 0.05f

#define TRIANGLE_SCALE 10 // MODE1: 45 | MODE2: 10

#define PARTICLE_BOUNDS 0.01 // MODE1: 15.0f | MODE2: 0.01f
#define MAX_VELOCITY 35.0 // MODE1: 1.0f | MODE2: 35.0f
#define ALPHA_MIN 0.5f // Also used for z distance

#define BACKGROUND_SATURATION 0.25f

// Tracks the update loop
class FrameManager{
private:
    int last_time = 0;
    int ONE_SECOND = 1000;
public:
    float delta_time = 0;
    FrameManager();
    void update();
    void print_fps();
    void update_delta_time();
};
FrameManager::FrameManager(){
    last_time = 0;
}
void FrameManager::update(){
    float time = 0;
    float delta = (time - last_time);
    delta_time = delta/ONE_SECOND;
    last_time = time;
}
void FrameManager::print_fps(){
    
    int fps = ONE_SECOND/0 - last_time;
    // Clear screen
    for(int i=0; i<50; i++) std::cout<<"\n";
    // Print FPS
    std::cout<<"FPS: "<< fps << " ";
    for(int i=0; i<fps; i++) std::cout<<"|";
    std::cout<<"\n";
}

// Stores two floating point numbers relating to x, y positions
class Vector2f{
public:
    float x, y;
public:
    Vector2f(float, float);
    Vector2f();
};
Vector2f::Vector2f(float a, float b) : x(a), y(b) {}
Vector2f::Vector2f() : x(0), y(0) {}

// Stores four floating point numbers relating to w, x, y, z positions
class Vector4f{
public:
    float w, x, y, z;
public:
    Vector4f(float, float, float, float);
    Vector4f();
};
Vector4f::Vector4f(float a, float b, float c, float d) : w(a), x(b), y(c), z(d) {}
Vector4f::Vector4f() : w(0), x(0), y(0), z(0) {}

class Shape {
public:
    GLenum drawMethod;
    Vector4f color;
public:
    void start_drawing(GLenum, Vector4f);
};
void Shape::start_drawing(GLenum draw_method, Vector4f color = Vector4f(0,0,0,1)){
    // Helper method for polygons
    glBegin(draw_method);
    glColor4f(color.w, color.x, color.y, color.z);
}

// Represents a Triangle with three verticies.
class Triangle : Shape {
public:
    Vector2f
        v1 = Vector2f(),
        v2 = Vector2f(),
        v3 = Vector2f();
        GLenum drawMethod;
        Vector4f color;
public:
    void draw();
    void drawMultiColor(Vector4f, Vector4f, Vector4f);
    void set(Vector2f, Vector2f, Vector2f, GLenum, Vector4f);
};
void Triangle::set(Vector2f v1, Vector2f v2, Vector2f v3, GLenum drawMethod, Vector4f color = Vector4f(0,0,0,1)){
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->color = color;
    this->drawMethod = drawMethod;
}
void Triangle::draw() {
    start_drawing(drawMethod, color);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
        glVertex2f(v3.x, v3.y);
    glEnd();
}
void Triangle::drawMultiColor(Vector4f c1, Vector4f c2, Vector4f c3){
    start_drawing(drawMethod);
    glColor4f(c1.w, c1.x, c1.y, c1.z);
    glVertex2f(v1.x, v1.y);
    glColor4f(c2.w, c2.x, c2.y, c2.z);
    glVertex2f(v2.x, v2.y);
    glColor4f(c3.w, c3.x, c3.y, c3.z);
    glVertex2f(v3.x, v3.y);
    glEnd();
}

// Arc given center, radius and degrees it spans
class Arc : Shape{
public:
    Vector2f center = Vector2f();
    float radius;
    int startDeg, endDeg;
    Vector4f color = Vector4f(0,0,0,1);
    GLenum drawMethod;
public:
    void draw();
    void set(Vector2f, float, int, int, GLenum, Vector4f);
};
void Arc::set(Vector2f center, float radius, int startDeg, int endDeg,
              GLenum drawMethod = GL_LINE_STRIP , Vector4f color = Vector4f(0,0,0,1)){
    this->center = center;
    this->radius = radius;
    this->startDeg = startDeg;
    this->endDeg = endDeg;
    this->color = color;
    this->drawMethod = drawMethod;
}

void Arc::draw(){
    start_drawing(drawMethod, color);
    for(int i=startDeg; i<=endDeg; i++){
        glVertex2f(center.x+radius*sin(2.0*M_PI*i/360.0),
                   center.y+radius*cos(2.0*M_PI*i/360.0));
    }
    glEnd();
}

class Particle : public Arc{
private:
    Vector2f velocity = Vector2f();
    float drag;
public:
    void set_particle(Vector2f, float);
    void traverse(float);
};
void Particle::set_particle(Vector2f velocity, float drag){
    this->velocity = velocity;
    this->drag = drag;
}
void Particle::traverse(float delta_time){
    float deltaX = velocity.x * delta_time;
    float deltaY = velocity.y * delta_time;
    center = Vector2f(center.x + deltaX, center.y + deltaY);
    float multiplier = 1.0 - drag * delta_time;
    velocity = Vector2f(velocity.x * multiplier, velocity.y * multiplier);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Begin psedu-class (render)
////////////////////////////////////////////////////////////////////////////

Particle particles[NUM_PARTICLES];
float rand_nums[NUM_PARTICLES];
FrameManager* frame_manager;

float generate_random_number(float precision = 100, float lower = 0, float upper = 1){
    return rand()%(int)precision/precision * (upper - lower) + lower;
}

void init_particles(){
    for(int i=0; i<NUM_PARTICLES; i++){
        const float ANGLE_PRECISION = 36000.0f;
        const float VELOCITY_PRECISION = 10000.0f;
        const float POSITION_PRECISION = 100.0f;
        const float ALPHA_PRECISION = 100.0f;
        const float DRAG_PRECISION = 100.0f;
        
        const float DRAG_LOWER_PERCENT = 0.25f;
        const float DRAG_UPPER_PERCENT = 0.75f;
        
        const float ARC_ANGLE = 360;
        
        // Random position
        float angle = (rand()%(int)ANGLE_PRECISION) / ANGLE_PRECISION * 2 * M_PI;
        float posX = generate_random_number(POSITION_PRECISION, 0, PARTICLE_BOUNDS) * cos(angle);
        float posY = generate_random_number(POSITION_PRECISION, 0, PARTICLE_BOUNDS) * sin(angle);
        
        // Random velocities
        angle = generate_random_number(ANGLE_PRECISION, 0, 1) * 2 * M_PI;
        float velocityX = generate_random_number(VELOCITY_PRECISION, 0, MAX_VELOCITY) * cos(angle);
        float velocityY = generate_random_number(VELOCITY_PRECISION, 0, MAX_VELOCITY) * sin(angle);
        
        // Random alpha with boost
        float alpha = generate_random_number(ALPHA_PRECISION, 0, 1) + ALPHA_MIN;
        alpha = alpha > 1 ? 1 : alpha;
        
        // Random drag
        float drag = generate_random_number(DRAG_PRECISION, DRAG_LOWER_PERCENT, DRAG_UPPER_PERCENT) * PARTICLE_DRAG;
        
        // Set states
        particles[i].set(Vector2f(posX, posY), PARTICLE_SIZE * alpha, 0, ARC_ANGLE, GL_POLYGON, Vector4f(1,1,1,alpha));
        particles[i].set_particle(Vector2f(PARTICLE_SPEED * velocityX * alpha, PARTICLE_SPEED * velocityY * alpha), drag);
    }
}

// Initilisations before render loop
void start_render(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    glTranslatef(0, Y_OFFSET, 0);
    generate_random_number();
    frame_manager = new FrameManager();
    init_particles();
}

// Tidies up and updates states after one render loop
void finish_render(){
    //frame_manager->print_fps();
    //frame_manager->update();
    
    // Re-render to start next frame
   // glutPostRedisplay();
}

void draw_particles(){
    for(int i=0; i<NUM_PARTICLES; i++){
        particles[i].draw();
        particles[i].traverse(frame_manager->delta_time);
    }
}

void draw_link(int i, int j){
    const float OPACITY_SCALE = 0.25f;
    float x1 = particles[i].center.x;
    float x2 = particles[j].center.x;
    float y1 = particles[i].center.y;
    float y2 = particles[j].center.y;
    float deltaX = x1 - x2;
    float deltaY = y1 - y2;
    float vicinitySq = PARTICLE_VICINITY * PARTICLE_VICINITY;
    float deltaSumSq = deltaX * deltaX + deltaY * deltaY;
    bool isNear = deltaSumSq < vicinitySq;
    if(isNear){
        glBegin(GL_LINES);
            glColor4f(1, 1, 1, (1 - deltaSumSq/vicinitySq) * OPACITY_SCALE * particles[i].color.z);
            glVertex2f(x1, y1);
            glColor4f(1, 1, 1, (1 - deltaSumSq/vicinitySq) * OPACITY_SCALE * particles[j].color.z);
            glVertex2f(x2, y2);
        glEnd();
    }
}

void draw_links(){
    // Can improve if using quadrants
    for(int i=0; i<NUM_PARTICLES; i++){
        for(int j=0; j<NUM_PARTICLES; j++){
            draw_link(i, j);
        }
    }
}

void set_triangle(Triangle *tri, float scale, GLenum type){
    float VERT_ANGLE = (M_PI/6.0);
    tri->set(Vector2f(-cos(VERT_ANGLE) * scale , -sin(VERT_ANGLE) * scale),
             Vector2f(0, 1 * scale),
             Vector2f(cos(VERT_ANGLE) * scale, -sin(VERT_ANGLE) * scale),
             type,
             Vector4f());
}

void draw_background(){
    Triangle* tri = new Triangle();
    const float small_to_large_ratio = 0.7f;
    
    // Draw outer triangle
    set_triangle(tri, TRIANGLE_SCALE, GL_POLYGON);
    tri->drawMultiColor(
                        Vector4f(0, 0, BACKGROUND_SATURATION, 1),
                        Vector4f(0, 0, 0.0f, 1),
                        Vector4f(BACKGROUND_SATURATION, 0, BACKGROUND_SATURATION, 1));
    
    // Draw inner triangle
    set_triangle(tri, TRIANGLE_SCALE * small_to_large_ratio, GL_POLYGON);
    tri->drawMultiColor(Vector4f(BACKGROUND_SATURATION, 0, BACKGROUND_SATURATION, 1),
                        Vector4f(0, 0, 0.0f, 1),
                        Vector4f(0, 0, BACKGROUND_SATURATION, 1));
    
    // Draw splitting line
    set_triangle(tri, TRIANGLE_SCALE * small_to_large_ratio, GL_LINE_LOOP);
    tri->drawMultiColor(Vector4f(1, 1, 1, 1),
                        Vector4f(1, 1, 1, 1),
                        Vector4f(1, 1, 1, 1));
    delete tri;
}

void main_render(){
    draw_background();
    draw_links();
    draw_particles();
}

void render(){
    main_render();
    finish_render();
}
