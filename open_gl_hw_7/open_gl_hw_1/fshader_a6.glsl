#version 150
in  vec4 color;
in vec2 st;
uniform sampler2D texMap;
out vec4 fColor;
void main()
{
	fColor = color * texture2D(texMap, st);// this reads the colors from the presently bound texture you sent over.
}