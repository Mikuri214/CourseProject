#pragma once

/// Init the OpenGL
void init( void );

/// Display the Object
void display( void );

/// Mouse Messenge
void mouse( int button, int state, int x, int y );

/// Motion Function
void motion( int x, int y );

/// Wheel Function
void wheel( int button, int dir, int x, int y );

/// Keyboard Messenge
void keyboard( unsigned char key, int x, int y );

/// Idle Function
void idle( void );


void reshape( int width, int height );