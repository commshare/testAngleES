attribute vec4 a_Position; 
attribute vec2 a_TexCoord; 
uniform mat4 u_MvpMatrix; 
varying vec2 v_TexCoord; 

void main() { 
    v_TexCoord = a_Position.xy; 
    gl_Position =  a_Position; 
}
