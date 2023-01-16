#pragma once

#include <glm/glm.hpp>
#include "stb_image.h"

namespace optic {
    // Initial Values
    static float reflectivity = 0.;
    static float refractivity = 0.;
    static float coefRefraction = 10.;
    /* Refraction indices:
	Air:      1.0
	Water:    1.33
	Ice:      1.309
	Glass:    1.52
	Diamond:  2.42
    */
}

namespace values
{
    static constexpr int MAX_POINT_LIGHTS = 10;
    static constexpr int MAX_SPOT_LIGHTS = 10;
    static constexpr int MAX_AREA_LIGHTS = 10;
}

namespace image
{
    //classic image
    static constexpr char white[128] = "../../image/Textures/white.png";
    static constexpr char wood[128] = "../../image/Textures/woodFloor.png";
    static constexpr char damier[128] = "../../image/Textures/damier.png";
    static constexpr char container[128] = "../../image/Textures/container.jpg";
    static constexpr char concrete[128] = "../../image/Textures/concrete.png";
    static constexpr char brick[128] = "../../image/Textures/brick.png";
    static constexpr char dirt[128] = "../../image/Textures/dirt.png";
    static constexpr char point1[128] = "../../image/Textures/point1.jpg";
    static constexpr char point2[128] = "../../image/Textures/point2.jpg";

    //image with normal
    static constexpr char brickwall[128] = "../../image/Textures/brickwall.jpg";
    static constexpr char brickwalln[128] = "../../image/NormalMap/brickwall_normal.jpg";

    //image with paralax
    static constexpr char brick2[128] = "../../image/Textures/bricks2.jpg";
    static constexpr char brick2n[128] = "../../image/NormalMap/bricks2_normal.jpg";
    static constexpr char brick2d[128] = "../../image/Parallax/bricks2_disp.jpg";

    static constexpr char toy[128] = "../../image/Textures/toy_box.png";
    static constexpr char toyn[128] = "../../image/NormalMap/toy_box_normal.png";
    static constexpr char toyd[128] = "../../image/Parallax/toy_box_disp.png";
}

namespace geometry
{
	static constexpr int plane = 0;
    static constexpr int domino = 1;
	static constexpr int cube = 2;
    static constexpr int sphere = 3;
    static constexpr int bunny = 4;
    static constexpr int plane3D = 5;
}

namespace geometryPath
{
    static constexpr char sphere[128] = "../../objects/sphere.obj";
	static constexpr char cube[128] = "../../objects/cube.obj";
	static constexpr char plane[128] = "../../objects/plane.obj";
	static constexpr char plane2[128] = "../../objects/plane2.obj";
    static constexpr char domino[128] = "../../objects/domino.obj";
    static constexpr char bunny[128] = "../../objects/bunny_big.obj";

}


namespace shaderfiles
{
    static constexpr char mainVertex[128] = "../../src/Shaders/main.vs";
    static constexpr char mainFrag[128] = "../../src/Shaders/main.fs";
    static constexpr char lightObjectVertex[128] = "../../src/Shaders/light_object.vs";
    static constexpr char lightObjectFrag[128] = "../../src/Shaders/light_object.fs";
    static constexpr char shadowMapVertex[128] = "../../src/Shaders/directional_shadow_map.vs";
    static constexpr char shadowMapFrag[128] = "../../src/Shaders/directional_shadow_map.fs";
    static constexpr char bumpMapVertex[128] = "../../src/Shaders/bumpmap.vs";
    static constexpr char bumpMapFrag[128] = "../../src/Shaders/bumpmap.fs";
    static constexpr char paralaxMapVertex[128] = "../../src/Shaders/paralax.vs";
    static constexpr char paralaxMapFrag[128] = "../../src/Shaders/paralax.fs";
    static constexpr char omniShadowFrag[128] = "../../src/Shaders/omni_shadow_map.fs";
    static constexpr char omniShadowVertex[128] = "../../src/Shaders/omni_shadow_map.vs";
    static constexpr char omniShadowGeom[128] = "../../src/Shaders/omni_shadow_map.geom";
} 

namespace general
{
    static constexpr glm::vec3 sceneSize = glm::vec3(100, 100, 100); // size of the scene, the scene is a cube (where the shadow are rendered in). (shadow box)
    static constexpr float floorThickness = 0.1; // If too law, do not work (0.05 is a good value)
    static constexpr float pushingExponent = 1.5; 
    static constexpr float mainShadowResolution = 16384;
    static constexpr float pointShadowResolution = 2048;
    static constexpr float spotShadowResolution = 2048;
}

namespace ShaderType
{
    static constexpr int OBJECT = 0;
    static constexpr int BUMPMAP = 1;
    static constexpr int PARALLAX = 2;
    static constexpr int POINTLIGHT = 3;
    static constexpr int SPOTLIGHT = 4;
    static constexpr int AREALIGHT = 5;
}

namespace Ttime
{
    static constexpr int maxTime = 5*60; // (number of seconds, e.g. 24h = 86400 seconds)
    static constexpr float startTime = 6 * maxTime /24;
}

namespace dominoDim
{
    static constexpr float height = 2*1.;
    static constexpr float width = 2*0.5;
    static constexpr float thick = 2*0.175;
}
namespace bunnyDim
{
    static constexpr float height = 3.2;
    static constexpr float width = 2.5;
    static constexpr float thick = 4;
}

namespace PHYSIC
{
    static constexpr int ANY_TYPE = 0;
    static constexpr int MOVABLE = 1;
    static constexpr int UNMOVABLE = 2;
}

namespace color
{
    static constexpr glm::vec3 Maroon = glm::vec3(0.501961f, 0.0f, 0.0f);
    static constexpr glm::vec3 DarkRed = glm::vec3(0.545098f, 0.0f, 0.0f);
    static constexpr glm::vec3 Brown = glm::vec3(0.647059f, 0.164706f, 0.164706f);
    static constexpr glm::vec3 Firebrick = glm::vec3(0.698039f, 0.133333f, 0.133333f);
    static constexpr glm::vec3 Crimson = glm::vec3(0.862745f, 0.0784314f, 0.235294f);
    static constexpr glm::vec3 Red = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Tomato = glm::vec3(1.0f, 0.388235f, 0.278431f);
    static constexpr glm::vec3 Coral = glm::vec3(1.0f, 0.498039f, 0.313726f);
    static constexpr glm::vec3 IndianRed = glm::vec3(0.803922f, 0.360784f, 0.360784f);
    static constexpr glm::vec3 LightCoral = glm::vec3(0.941176f, 0.501961f, 0.501961f);
    static constexpr glm::vec3 DarkSalmon = glm::vec3(0.913725f, 0.588235f, 0.478431f);
    static constexpr glm::vec3 Salmon = glm::vec3(0.980392f, 0.501961f, 0.447059f);
    static constexpr glm::vec3 LightSalmon = glm::vec3(1.0f, 0.627451f, 0.478431f);
    static constexpr glm::vec3 OrangeRed = glm::vec3(1.0f, 0.270588f, 0.0f);
    static constexpr glm::vec3 DarkOrange = glm::vec3(1.0f, 0.54902f, 0.0f);
    static constexpr glm::vec3 Orange = glm::vec3(1.0f, 0.647059f, 0.0f);
    static constexpr glm::vec3 Gold = glm::vec3(1.0f, 0.843137f, 0.0f);
    static constexpr glm::vec3 DarkGoldenRod = glm::vec3(0.721569f, 0.52549f, 0.0431373f);
    static constexpr glm::vec3 GoldenRod = glm::vec3(0.854902f, 0.647059f, 0.12549f);
    static constexpr glm::vec3 PaleGoldenRod = glm::vec3(0.933333f, 0.909804f, 0.666667f);
    static constexpr glm::vec3 DarkKhaki = glm::vec3(0.741176f, 0.717647f, 0.419608f);
    static constexpr glm::vec3 Khaki = glm::vec3(0.941176f, 0.901961f, 0.54902f);
    static constexpr glm::vec3 Olive = glm::vec3(0.501961f, 0.501961f, 0.0f);
    static constexpr glm::vec3 Yellow = glm::vec3(1.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 YellowGreen = glm::vec3(0.603922f, 0.803922f, 0.196078f);
    static constexpr glm::vec3 DarkOliveGreen = glm::vec3(0.333333f, 0.419608f, 0.184314f);
    static constexpr glm::vec3 OliveDrab = glm::vec3(0.419608f, 0.556863f, 0.137255f);
    static constexpr glm::vec3 LawnGreen = glm::vec3(0.486275f, 0.988235f, 0.0f);
    static constexpr glm::vec3 ChartReuse = glm::vec3(0.498039f, 1.0f, 0.0f);
    static constexpr glm::vec3 GreenYellow = glm::vec3(0.678431f, 1.0f, 0.184314f);
    static constexpr glm::vec3 DarkGreen = glm::vec3(0.0f, 0.392157f, 0.0f);
    static constexpr glm::vec3 Green = glm::vec3(0.0f, 0.501961f, 0.0f);
    static constexpr glm::vec3 ForestGreen = glm::vec3(0.133333f, 0.545098f, 0.133333f);
    static constexpr glm::vec3 Lime = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 LimeGreen = glm::vec3(0.196078f, 0.803922f, 0.196078f);
    static constexpr glm::vec3 LightGreen = glm::vec3(0.564706f, 0.933333f, 0.564706f);
    static constexpr glm::vec3 PaleGreen = glm::vec3(0.596078f, 0.984314f, 0.596078f);
    static constexpr glm::vec3 DarkSeaGreen = glm::vec3(0.560784f, 0.737255f, 0.560784f);
    static constexpr glm::vec3 MediumSpringGreen = glm::vec3(0.0f, 0.980392f, 0.603922f);
    static constexpr glm::vec3 SpringGreen = glm::vec3(0.0f, 1.0f, 0.498039f);
    static constexpr glm::vec3 SeaGreen = glm::vec3(0.180392f, 0.545098f, 0.341176f);
    static constexpr glm::vec3 MediumAquaMarine = glm::vec3(0.4f, 0.803922f, 0.666667f);
    static constexpr glm::vec3 MediumSeaGreen = glm::vec3(0.235294f, 0.701961f, 0.443137f);
    static constexpr glm::vec3 LightSeaGreen = glm::vec3(0.12549f, 0.698039f, 0.666667f);
    static constexpr glm::vec3 DarkSlateGray = glm::vec3(0.184314f, 0.309804f, 0.309804f);
    static constexpr glm::vec3 Teal = glm::vec3(0.0f, 0.501961f, 0.501961f);
    static constexpr glm::vec3 DarkCyan = glm::vec3(0.0f, 0.545098f, 0.545098f);
    static constexpr glm::vec3 Aqua = glm::vec3(0.0f, 1.0f, 1.0f);
    static constexpr glm::vec3 Cyan = glm::vec3(0.0f, 1.0f, 1.0f);
    static constexpr glm::vec3 LightCyan = glm::vec3(0.878431f, 1.0f, 1.0f);
    static constexpr glm::vec3 DarkTurquoise = glm::vec3(0.0f, 0.807843f, 0.819608f);
    static constexpr glm::vec3 Turquoise = glm::vec3(0.25098f, 0.878431f, 0.815686f);
    static constexpr glm::vec3 MediumTurquoise = glm::vec3(0.282353f, 0.819608f, 0.8f);
    static constexpr glm::vec3 PaleTurquoise = glm::vec3(0.686275f, 0.933333f, 0.933333f);
    static constexpr glm::vec3 Aquamarine = glm::vec3(0.498039f, 1.0f, 0.831373f);
    static constexpr glm::vec3 PowderBlue = glm::vec3(0.690196f, 0.878431f, 0.901961f);
    static constexpr glm::vec3 CadetBlue = glm::vec3(0.372549f, 0.619608f, 0.627451f);
    static constexpr glm::vec3 SteelBlue = glm::vec3(0.27451f, 0.509804f, 0.705882f);
    static constexpr glm::vec3 CornflowerBlue = glm::vec3(0.392157f, 0.584314f, 0.929412f);
    static constexpr glm::vec3 DeepSkyBlue = glm::vec3(0.0f, 0.74902f, 1.0f);
    static constexpr glm::vec3 DodgerBlue = glm::vec3(0.117647f, 0.564706f, 1.0f);
    static constexpr glm::vec3 LightBlue = glm::vec3(0.678431f, 0.847059f, 0.901961f);
    static constexpr glm::vec3 SkyBlue = glm::vec3(0.529412f, 0.807843f, 0.921569f);
    static constexpr glm::vec3 LightSkyBlue = glm::vec3(0.529412f, 0.807843f, 0.980392f);
    static constexpr glm::vec3 MidnightBlue = glm::vec3(0.0980392f, 0.0980392f, 0.439216f);
    static constexpr glm::vec3 Navy = glm::vec3(0.0f, 0.0f, 0.501961f);
    static constexpr glm::vec3 DarkBlue = glm::vec3(0.0f, 0.0f, 0.545098f);
    static constexpr glm::vec3 MediumBlue = glm::vec3(0.0f, 0.0f, 0.803922f);
    static constexpr glm::vec3 Blue = glm::vec3(0.0f, 0.0f, 1.0f);
    static constexpr glm::vec3 RoyalBlue = glm::vec3(0.254902f, 0.411765f, 0.882353f);
    static constexpr glm::vec3 BlueViolet = glm::vec3(0.541176f, 0.168627f, 0.886275f);
    static constexpr glm::vec3 Indigo = glm::vec3(0.294118f, 0.0f, 0.509804f);
    static constexpr glm::vec3 DarkSlateBlue = glm::vec3(0.282353f, 0.239216f, 0.545098f);
    static constexpr glm::vec3 SlateBlue = glm::vec3(0.415686f, 0.352941f, 0.803922f);
    static constexpr glm::vec3 MediumSlateBlue = glm::vec3(0.482353f, 0.407843f, 0.933333f);
    static constexpr glm::vec3 MediumPurple = glm::vec3(0.576471f, 0.439216f, 0.858824f);
    static constexpr glm::vec3 DarkMagenta = glm::vec3(0.545098f, 0.0f, 0.545098f);
    static constexpr glm::vec3 DarkViolet = glm::vec3(0.580392f, 0.0f, 0.827451f);
    static constexpr glm::vec3 DarkOrchid = glm::vec3(0.6f, 0.196078f, 0.8f);
    static constexpr glm::vec3 MediumOrchid = glm::vec3(0.729412f, 0.333333f, 0.827451f);
    static constexpr glm::vec3 Purple = glm::vec3(0.501961f, 0.0f, 0.501961f);
    static constexpr glm::vec3 Thistle = glm::vec3(0.847059f, 0.74902f, 0.847059f);
    static constexpr glm::vec3 Plum = glm::vec3(0.866667f, 0.627451f, 0.866667f);
    static constexpr glm::vec3 Violet = glm::vec3(0.933333f, 0.509804f, 0.933333f);
    static constexpr glm::vec3 Magenta = glm::vec3(1.0f, 0.0f, 1.0f);
    static constexpr glm::vec3 Orchid = glm::vec3(0.854902f, 0.439216f, 0.839216f);
    static constexpr glm::vec3 MediumVioletRed = glm::vec3(0.780392f, 0.0823529f, 0.521569f);
    static constexpr glm::vec3 PaleVioletRed = glm::vec3(0.858824f, 0.439216f, 0.576471f);
    static constexpr glm::vec3 DeepPink = glm::vec3(1.0f, 0.0784314f, 0.576471f);
    static constexpr glm::vec3 HotPink = glm::vec3(1.0f, 0.411765f, 0.705882f);
    static constexpr glm::vec3 LightPink = glm::vec3(1.0f, 0.713726f, 0.756863f);
    static constexpr glm::vec3 Pink = glm::vec3(1.0f, 0.752941f, 0.796078f);
    static constexpr glm::vec3 AntiqueWhite = glm::vec3(0.980392f, 0.921569f, 0.843137f);
    static constexpr glm::vec3 Beige = glm::vec3(0.960784f, 0.960784f, 0.862745f);
    static constexpr glm::vec3 Bisque = glm::vec3(1.0f, 0.894118f, 0.768627f);
    static constexpr glm::vec3 BlanchedAlmond = glm::vec3(1.0f, 0.921569f, 0.803922f);
    static constexpr glm::vec3 Wheat = glm::vec3(0.960784f, 0.870588f, 0.701961f);
    static constexpr glm::vec3 CornSilk = glm::vec3(1.0f, 0.972549f, 0.862745f);
    static constexpr glm::vec3 LemonChiffon = glm::vec3(1.0f, 0.980392f, 0.803922f);
    static constexpr glm::vec3 LightGoldenRodYellow = glm::vec3(0.980392f, 0.980392f, 0.823529f);
    static constexpr glm::vec3 LightYellow = glm::vec3(1.0f, 1.0f, 0.878431f);
    static constexpr glm::vec3 SaddleBrown = glm::vec3(0.545098f, 0.270588f, 0.0745098f);
    static constexpr glm::vec3 Sienna = glm::vec3(0.627451f, 0.321569f, 0.176471f);
    static constexpr glm::vec3 Chocolate = glm::vec3(0.823529f, 0.411765f, 0.117647f);
    static constexpr glm::vec3 Peru = glm::vec3(0.803922f, 0.521569f, 0.247059f);
    static constexpr glm::vec3 SandyBrown = glm::vec3(0.956863f, 0.643137f, 0.376471f);
    static constexpr glm::vec3 BurlyWood = glm::vec3(0.870588f, 0.721569f, 0.529412f);
    static constexpr glm::vec3 Tan = glm::vec3(0.823529f, 0.705882f, 0.54902f);
    static constexpr glm::vec3 RosyBrown = glm::vec3(0.737255f, 0.560784f, 0.560784f);
    static constexpr glm::vec3 Moccasin = glm::vec3(1.0f, 0.894118f, 0.709804f);
    static constexpr glm::vec3 NavajoWhite = glm::vec3(1.0f, 0.870588f, 0.678431f);
    static constexpr glm::vec3 PeachPuff = glm::vec3(1.0f, 0.854902f, 0.72549f);
    static constexpr glm::vec3 MistyRose = glm::vec3(1.0f, 0.894118f, 0.882353f);
    static constexpr glm::vec3 LavenderBlush = glm::vec3(1.0f, 0.941176f, 0.960784f);
    static constexpr glm::vec3 Linen = glm::vec3(0.980392f, 0.941176f, 0.901961f);
    static constexpr glm::vec3 OldLace = glm::vec3(0.992157f, 0.960784f, 0.901961f);
    static constexpr glm::vec3 PapayaWhip = glm::vec3(1.0f, 0.937255f, 0.835294f);
    static constexpr glm::vec3 SeaShell = glm::vec3(1.0f, 0.960784f, 0.933333f);
    static constexpr glm::vec3 MintCream = glm::vec3(0.960784f, 1.0f, 0.980392f);
    static constexpr glm::vec3 SlateGray = glm::vec3(0.439216f, 0.501961f, 0.564706f);
    static constexpr glm::vec3 LightSlateGray = glm::vec3(0.466667f, 0.533333f, 0.6f);
    static constexpr glm::vec3 LightSteelBlue = glm::vec3(0.690196f, 0.768627f, 0.870588f);
    static constexpr glm::vec3 Lavender = glm::vec3(0.901961f, 0.901961f, 0.980392f);
    static constexpr glm::vec3 FloralWhite = glm::vec3(1.0f, 0.980392f, 0.941176f);
    static constexpr glm::vec3 AliceBlue = glm::vec3(0.941176f, 0.972549f, 1.0f);
    static constexpr glm::vec3 GhostWhite = glm::vec3(0.972549f, 0.972549f, 1.0f);
    static constexpr glm::vec3 Honeydew = glm::vec3(0.941176f, 1.0f, 0.941176f);
    static constexpr glm::vec3 Ivory = glm::vec3(1.0f, 1.0f, 0.941176f);
    static constexpr glm::vec3 Azure = glm::vec3(0.941176f, 1.0f, 1.0f);
    static constexpr glm::vec3 Snow = glm::vec3(1.0f, 0.980392f, 0.980392f);
    static constexpr glm::vec3 Black = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 DimGrey = glm::vec3(0.411765f, 0.411765f, 0.411765f);
    static constexpr glm::vec3 Grey = glm::vec3(0.501961f, 0.501961f, 0.501961f);
    static constexpr glm::vec3 DarkGrey = glm::vec3(0.662745f, 0.662745f, 0.662745f);
    static constexpr glm::vec3 Silver = glm::vec3(0.752941f, 0.752941f, 0.752941f);
    static constexpr glm::vec3 LightGrey = glm::vec3(0.827451f, 0.827451f, 0.827451f);
    static constexpr glm::vec3 Gainsboro = glm::vec3(0.862745f, 0.862745f, 0.862745f);
    static constexpr glm::vec3 WhiteSmoke = glm::vec3(0.960784f, 0.960784f, 0.960784f);
    static constexpr glm::vec3 White = glm::vec3(1.0f);
}



