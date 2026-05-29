VARYING vec4 vInstanceColor;
VARYING vec4 vCustomData;

// IA Usage
// Utilisation: Generating "Cheat Sheet"
// Model: Claude Sonnet 4.6
// Prompt: Can you provide a comprehensive list of all built-in variables, 
//		   functions, macros, uniforms, attributes, varyings, and Qt-specific 
//		   shader features available in Qt Quick 3D GLSL shaders.
// Developper: Joël Mailly-Niquay
// Notes: Some where incorrect or outdated

void MAIN()
{
	vInstanceColor = INSTANCE_COLOR;
	vCustomData = INSTANCE_DATA;
}