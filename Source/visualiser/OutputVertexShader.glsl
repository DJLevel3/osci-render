std::string outputVertexShader = R"(

attribute vec2 aPos;
varying vec2 vTexCoord;
varying vec2 vTexCoordCanvas;
uniform float uResizeForCanvas;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTexCoord = (0.5 * vec2(aPos.x, -aPos.y) + 0.5);
    vTexCoordCanvas = vTexCoord * uResizeForCanvas;
}

)";
