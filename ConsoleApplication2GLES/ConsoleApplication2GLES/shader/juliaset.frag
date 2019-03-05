precision mediump float;

varying vec2 xy;

uniform vec2 julia_c;
const int MaxIt = 32;
const vec3 ColorF = vec3(127.0, 25.0, 190.0) / 255.0;
const vec3 ColorT = vec3(1.0, 1.0, 1.0);
const vec3 ColorV = ColorT - ColorF;

void main() {
    float x = xy.x;
    float y = xy.y;
    float qx = pow(x, 2.0);
    float qy = pow(y, 2.0);
    int step = 1;
    for(int i=0;i<MaxIt;i++) {
        y = 2.0 * x * y + julia_c.y;
        x = qx - qy + julia_c.x;
        qx = pow(x, 2.0);
        qy = pow(y, 2.0);
        step++;
        if(qx + qy > 4.0) {
            break;
        }
    }
    float k = float(step) / float(MaxIt);
    gl_FragColor = vec4(ColorF + ColorV * k, 1.0);
}
