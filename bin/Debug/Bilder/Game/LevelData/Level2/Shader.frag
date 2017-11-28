#version 120
// Shader für Licht Effekte auf Tilemap
// Snake Game by MOP

// Texturen
uniform sampler2D Diffuse_Textur;
uniform sampler2D Normal_Textur;
uniform sampler2D Spiegel_Textur;
uniform float Time;
in vec2 tex_coord;

// Konstanten
const vec4 Licht_Farbe = vec4(0.8f, 0.8f, 0.8f, 1.0f);
const vec3 Ambient_Farbe = vec3(0.6f, 0.6f, 1.0f);
const vec4 Spiegelungs_Farbe = vec4(0.4f, 0.4f, 0.4f, 1.0f);
const vec2 Aufloesung = vec2(600.0f,700.0f);
const int MAX_LIGHTS = 10;
const float Desaturate_Factor = 0.8f;

// Licht Daten
uniform vec3 Licht_Position[MAX_LIGHTS];
uniform vec3 Kegel_Position[MAX_LIGHTS];
uniform float Licht_Kegel_Aktiv[MAX_LIGHTS];
uniform float FBM_Kegel_Aktiv[MAX_LIGHTS];
uniform float Ambient_Staerke[MAX_LIGHTS];
uniform float Licht_Kegel_Winkel[MAX_LIGHTS];
uniform float Licht_Aktiv[MAX_LIGHTS];
uniform int Anzahl_Lichter;

// Other Stuff
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
float FBM(float x, float f, float a)
{
	float Value = 0.0f;
	float Amplitude = a;
	float Frequency = f;
	for(int i = 0; i<10; i++)
    {
        Value += Noise(x * Frequency) * Amplitude;
        Frequency *= f;
        Amplitude *= 0.5f;
    }

    return Value;
}

// Hauptfunktion
void main()
{
    // Get Colors from textures
    vec4 Diffuse_Farbe = texture2D(Diffuse_Textur, gl_TexCoord[0].xy);
    vec3 Normal_Farbe = texture2D(Normal_Textur, gl_TexCoord[0].xy).rgb;
    vec4 Spiegel_Farbe = texture2D(Spiegel_Textur, gl_TexCoord[0].xy);
    vec3 End_Farbe = vec3(0.0f);

    // Iterate over all setted lights
    for(int count = 0; count<Anzahl_Lichter; count++)
    {
        // Get direction and fix it
        vec3 Licht_Richtung = vec3(Licht_Position[count].xy - (gl_FragCoord.xy/Aufloesung.xy), Licht_Position[count].z);
        Licht_Richtung.x *= Aufloesung.x / Aufloesung.y;

        // Calculate attentuation
        float Licht_Laenge = length(Licht_Richtung);
        float Daempfung = 1.0 / ( 0.1f + (0.2f*Licht_Laenge) + (1.5f*pow(Licht_Laenge, 2)) );

        // normnalize vectors
        vec3 N = normalize(Normal_Farbe * 2.0f - 1.0f);
        vec3 L = normalize(Licht_Richtung);

        // Light cone active?
        if(Licht_Kegel_Aktiv[count] > 0.99f)
        {
            vec3 Kegel_Richtung = vec3(Kegel_Position[count].xy - (gl_FragCoord.xy/Aufloesung.xy), Kegel_Position[count].z);
            Kegel_Richtung.x *= Aufloesung.x / Aufloesung.y;

            float Aktueller_Licht_Winkel = degrees(acos(dot(-L, normalize(Kegel_Richtung))));
            if(Aktueller_Licht_Winkel > Licht_Kegel_Winkel[count])
            {
                Daempfung = 0.0f;
            }
        }

        // Calculate diffuse
        float Diffuse_Factor = max(dot(N,L), 0.0f);

        // Calculate specular
        vec4 Spiegel = vec4(0.0f);
        if(Diffuse_Factor > 0.0f)
        {
            vec3 Halber_Weg = normalize(Licht_Richtung + vec3(0.5f, 0.5f, 0.5f));
            float Spiegel_Faktor = max(dot(N, Halber_Weg), 0.0f);
            Spiegel = Spiegel_Faktor * Spiegelungs_Farbe * Spiegel_Farbe;
        }

        // Motion active?
        vec3 Diffuse = (Licht_Farbe.rgb * Licht_Farbe.a) * Diffuse_Factor;
        if(FBM_Kegel_Aktiv[count] > 0.99f)
        {
            Diffuse *= FBM(Time, 2.0f, 0.75f);
        }

        // Light active?
        if(Licht_Aktiv[count] > 0.99f)
        {
            End_Farbe += (Spiegel.rgb + Diffuse_Farbe.rgb) * ((Ambient_Farbe.rgb * Ambient_Staerke[count]) + Diffuse * Daempfung);
        }
        else
        {
            End_Farbe = vec3(0.0f, 0.0f, 0.0f);
        }
    }

    // Game Paused?
    if(Desaturate)
    {
        // Desature
        float Intensity = (0.3f * End_Farbe.r) + (0.59f * End_Farbe.g) + (0.11f * End_Farbe.b);

        End_Farbe.r = (Intensity * Desaturate_Factor * FBM(Time, 0.25f, 0.95f)) + (End_Farbe.r * (1.0f - Desaturate_Factor));
        End_Farbe.g = (Intensity * Desaturate_Factor * FBM(Time, 0.25f, 0.95f)) + (End_Farbe.g * (1.0f - Desaturate_Factor));
        End_Farbe.b = (Intensity * Desaturate_Factor * FBM(Time, 0.25f, 0.95f)) + (End_Farbe.b * (1.0f - Desaturate_Factor));
    }

    // Set Pixels
    gl_FragColor = gl_Color * vec4(End_Farbe, Diffuse_Farbe.a);
}


