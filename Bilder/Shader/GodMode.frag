#version 120
// Shader for GodMode Item

// Input Texture
uniform sampler2D Texture;
uniform float Time;

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
        Frequency *= 1.5f;
        Amplitude *= 0.5f;
    }

    return Value;
}

// Main Function
void main()
{
    gl_FragColor = texture2D(Texture, gl_TexCoord[0].xy) * FBM(Time);
}
