precision mediump float;

void main() { 
    float d = distance(gl_PointCoord, vec2(0.5, 0.5));
    if(d < 0.5) {
        gl_FragColor = vec4(0.8, 0.1, 0.3, 1.0);
    } else { 
        discard; 
    }
}