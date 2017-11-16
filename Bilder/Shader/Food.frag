#version 120
// Shader for Food Item

// Constants
const float Desaturate_Factor = 1.0f;

// Input Texture
uniform sampler2D Texture;
uniform float Time;
uniform bool Desaturate;

// Functions
// Noise
float Noise(float x)
{
	float i = floor(x);
	float f = fract(x);
	float u = f * f * (3.0f - 2.0f * f);
	return mix(fract(sin(i) * 43758.5453123f), fract(sin(i + 1.0f) * 43758.5453123f), u);
}
// Fractional Brown Motion
float FBM(float x)
{
	float Value = 0.0f;
	float Amplitude = 0.75f;
	float Frequency = 2.0f;
	for(int i = 0; i<10; i++)
    {
        Value += Noise(x * Frequency) * Amplitude;
        Frequency *= 1.25f;
        Amplitude *= 0.3f;
    }

    return Value;
}

// Main Function
void main()
{
    // Get Color
    vec4 Tex_Color = texture2D(Texture, gl_TexCoord[0].xy);

    // Make Flicker
    Tex_Color *= FBM(Time);

    // Desature
    if(Desaturate)
    {
        float Intensity = (0.3f * Tex_Color.r) + (0.59f * Tex_Color.g) + (0.11f * Tex_Color.b);

        Tex_Color.r = (Intensity * Desaturate_Factor) + (Tex_Color.r * (1.0f - Desaturate_Factor));
        Tex_Color.g = (Intensity * Desaturate_Factor) + (Tex_Color.g * (1.0f - Desaturate_Factor));
        Tex_Color.b = (Intensity * Desaturate_Factor) + (Tex_Color.b * (1.0f - Desaturate_Factor));
    }

    gl_FragColor = Tex_Color;
}
