xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 125;
 -2.69735;28.07130;11.14628;,
 1.57595;32.84826;5.47113;,
 -3.10310;32.90334;3.84393;,
 -10.83218;28.29220;-1.21299;,
 -5.69237;31.64277;-3.14442;,
 -8.86727;27.83499;-9.27462;,
 -3.58513;27.64626;-11.94817;,
 -0.15908;33.13802;-4.19396;,
 5.17854;31.22329;-7.73959;,
 -7.67210;30.51284;7.73639;,
 -7.99304;30.56621;2.11268;,
 -3.83316;19.50052;14.83925;,
 -8.49803;25.77214;10.81237;,
 -12.57913;20.86059;10.17576;,
 -8.40324;19.86289;12.91517;,
 -10.29406;4.94467;10.08501;,
 -6.39614;11.69048;14.68521;,
 -10.79258;13.77670;12.40491;,
 -14.37128;11.64209;5.91107;,
 -13.50185;6.52222;4.88814;,
 -16.07230;9.30005;-1.16719;,
 -18.29992;18.13025;0.30484;,
 -15.09478;17.55836;6.76954;,
 -3.84826;-0.59575;9.15055;,
 -8.69428;0.11722;4.68226;,
 -12.73035;3.38547;1.46516;,
 -17.75302;21.57033;3.67339;,
 -16.38711;18.61576;-5.99395;,
 -17.51203;13.10200;-5.54710;,
 -12.43364;26.53385;7.59761;,
 -15.95011;25.78295;2.11490;,
 6.88602;18.60820;-18.75368;,
 2.06425;12.25239;-17.45240;,
 1.55282;20.21387;-18.32650;,
 -3.03712;17.96647;-16.23802;,
 -11.94130;21.18164;-11.85948;,
 -7.35205;21.99620;-15.20870;,
 -9.39592;16.91812;-14.64414;,
 -14.94172;13.58801;-11.96584;,
 9.52171;13.80949;-18.45894;,
 10.30956;22.54879;-13.33466;,
 5.81655;25.04006;-17.40493;,
 8.87565;28.45232;-11.41175;,
 -0.36340;25.81247;-16.14419;,
 12.93978;22.92325;-9.14973;,
 18.28795;18.57471;-7.82553;,
 16.92794;22.59972;-4.53330;,
 15.93452;14.46099;-16.50631;,
 14.90049;18.71481;-14.24191;,
 18.64170;12.14890;-10.93157;,
 14.03289;27.36119;0.78560;,
 10.32815;31.82601;0.30052;,
 5.17854;31.22329;-7.73959;,
 4.71692;35.12170;0.79060;,
 20.33081;14.53994;-3.61001;,
 21.16439;17.42166;1.07250;,
 16.51786;19.69097;9.93239;,
 19.22118;21.43335;2.86673;,
 14.00337;25.02557;8.18241;,
 10.78026;29.56114;-5.93372;,
 20.64915;15.11173;5.14931;,
 19.82217;8.33592;-6.35774;,
 19.57809;8.95076;0.12357;,
 17.77602;3.13120;-6.35312;,
 17.86274;9.85438;7.92526;,
 14.74423;4.11140;2.52910;,
 9.59698;6.38379;-15.83348;,
 -0.05199;8.17550;-15.33548;,
 1.23399;1.76524;-11.11109;,
 14.85431;7.78335;-13.30466;,
 -13.66315;25.81907;-6.13627;,
 -3.03712;17.96647;-16.23802;,
 -13.60258;8.63695;-10.59545;,
 -6.80441;9.54701;-13.28107;,
 -18.47688;23.75591;-1.85052;,
 -8.77430;3.47608;-8.86960;,
 -4.60190;5.22869;-11.67841;,
 -3.12764;-2.15867;-4.73326;,
 -3.95257;-2.91237;1.62598;,
 -8.54966;0.19489;-3.10052;,
 -3.32728;0.72886;-8.76584;,
 -12.71825;4.62670;-5.59280;,
 13.68751;0.71969;-11.40800;,
 15.00881;-1.39034;-5.97794;,
 7.57307;-0.89260;-9.41428;,
 -4.60190;5.22869;-11.67841;,
 -3.12764;-2.15867;-4.73326;,
 -3.32728;0.72886;-8.76584;,
 3.10905;-2.92640;-3.37554;,
 10.33936;-3.06741;-4.11534;,
 9.27317;-1.42147;1.48836;,
 1.23399;1.76524;-11.11109;,
 3.10905;-2.92640;-3.37554;,
 3.70088;-2.80706;5.53512;,
 2.68742;30.52326;10.98185;,
 10.26480;24.49616;13.48266;,
 3.26868;24.98391;16.35968;,
 7.40101;21.94313;17.47807;,
 7.86622;31.98814;6.16538;,
 12.26643;17.25013;14.39459;,
 6.06384;15.85896;17.23425;,
 16.43010;14.95033;10.85713;,
 14.64321;7.47100;12.60459;,
 11.55053;11.91205;15.17885;,
 5.92767;7.45115;17.35058;,
 11.26434;6.81548;16.15004;,
 11.11221;2.34225;8.28466;,
 8.26069;2.80677;15.06677;,
 2.77064;11.69705;17.56359;,
 2.67493;-1.08844;12.82997;,
 -3.94805;3.43562;13.32391;,
 2.72292;2.77189;16.81965;,
 -0.43634;7.97461;17.02188;,
 -1.00488;14.82340;17.11260;,
 7.25817;29.45484;11.60553;,
 -0.15908;33.13802;-4.19396;,
 -5.69237;31.64277;-3.14442;,
 -3.10310;32.90334;3.84393;,
 4.63182;28.97946;-13.31287;,
 -3.58513;27.64626;-11.94817;,
 4.63182;28.97946;-13.31287;,
 2.06425;12.25239;-17.45240;,
 -0.05199;8.17550;-15.33548;,
 -6.80441;9.54701;-13.28107;,
 -7.35205;21.99620;-15.20870;;
 
 214;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,0,2;,
 3;9,2,10;,
 3;4,3,10;,
 3;5,4,6;,
 3;11,0,12;,
 3;12,13,14;,
 3;15,16,17;,
 3;17,14,13;,
 3;17,18,15;,
 3;18,19,15;,
 3;20,18,21;,
 3;11,12,14;,
 3;16,11,17;,
 3;17,22,18;,
 3;23,15,24;,
 3;24,15,25;,
 3;13,26,22;,
 3;21,27,28;,
 3;20,21,28;,
 3;29,3,30;,
 3;13,29,26;,
 3;18,20,19;,
 3;31,32,33;,
 3;33,32,34;,
 3;35,5,36;,
 3;37,38,35;,
 3;39,32,31;,
 3;40,41,42;,
 3;5,6,36;,
 3;33,34,43;,
 3;40,42,44;,
 3;45,44,46;,
 3;47,48,49;,
 3;46,44,50;,
 3;51,52,53;,
 3;54,45,55;,
 3;49,45,54;,
 3;56,57,58;,
 3;59,50,44;,
 3;54,55,60;,
 3;61,62,63;,
 3;62,64,65;,
 3;32,66,67;,
 3;39,31,48;,
 3;67,66,68;,
 3;66,32,39;,
 3;69,66,39;,
 3;4,7,6;,
 3;5,35,70;,
 3;37,36,71;,
 3;36,37,35;,
 3;72,38,37;,
 3;70,35,27;,
 3;73,72,37;,
 3;18,22,26;,
 3;29,30,26;,
 3;21,18,26;,
 3;26,74,21;,
 3;26,30,74;,
 3;70,74,3;,
 3;3,74,30;,
 3;72,73,75;,
 3;75,73,76;,
 3;77,78,79;,
 3;80,77,79;,
 3;72,75,81;,
 3;78,24,79;,
 3;27,35,38;,
 3;25,20,81;,
 3;19,20,25;,
 3;79,81,75;,
 3;70,27,74;,
 3;21,74,27;,
 3;38,28,27;,
 3;80,75,76;,
 3;72,28,38;,
 3;80,79,75;,
 3;25,15,19;,
 3;72,20,28;,
 3;25,79,24;,
 3;31,41,40;,
 3;44,42,59;,
 3;41,33,43;,
 3;40,48,31;,
 3;39,48,47;,
 3;69,39,47;,
 3;49,54,61;,
 3;82,83,84;,
 3;68,85,67;,
 3;66,84,68;,
 3;68,86,87;,
 3;88,68,84;,
 3;88,84,89;,
 3;83,90,89;,
 3;90,88,89;,
 3;54,62,61;,
 3;65,63,62;,
 3;77,91,92;,
 3;89,84,83;,
 3;93,88,90;,
 3;2,53,7;,
 3;1,0,94;,
 3;51,59,52;,
 3;2,4,10;,
 3;95,96,97;,
 3;57,50,58;,
 3;2,1,53;,
 3;58,50,98;,
 3;98,53,1;,
 3;99,95,97;,
 3;95,99,56;,
 3;96,100,97;,
 3;56,99,101;,
 3;101,102,64;,
 3;46,50,57;,
 3;101,99,103;,
 3;95,56,58;,
 3;104,103,100;,
 3;103,104,105;,
 3;101,103,102;,
 3;65,64,106;,
 3;107,102,105;,
 3;54,60,62;,
 3;101,64,60;,
 3;105,102,103;,
 3;64,62,60;,
 3;99,100,103;,
 3;100,108,104;,
 3;107,109,106;,
 3;109,93,106;,
 3;106,64,102;,
 3;110,111,112;,
 3;111,104,112;,
 3;108,112,104;,
 3;108,100,113;,
 3;112,16,110;,
 3;106,90,65;,
 3;97,100,99;,
 3;106,102,107;,
 3;93,109,23;,
 3;111,107,104;,
 3;16,112,113;,
 3;23,109,110;,
 3;106,93,90;,
 3;15,110,16;,
 3;24,78,23;,
 3;110,15,23;,
 3;11,16,113;,
 3;23,78,93;,
 3;101,60,56;,
 3;57,60,55;,
 3;60,57,56;,
 3;55,45,57;,
 3;45,46,57;,
 3;49,61,63;,
 3;44,45,48;,
 3;83,82,63;,
 3;48,40,44;,
 3;48,45,49;,
 3;65,83,63;,
 3;66,69,82;,
 3;82,69,63;,
 3;84,66,82;,
 3;78,77,92;,
 3;78,92,93;,
 3;96,0,11;,
 3;100,96,113;,
 3;113,96,11;,
 3;96,94,0;,
 3;98,1,94;,
 3;94,114,98;,
 3;113,112,108;,
 3;51,53,98;,
 3;115,116,117;,
 3;118,42,41;,
 3;52,42,118;,
 3;42,52,59;,
 3;43,119,118;,
 3;6,8,120;,
 3;51,50,59;,
 3;58,114,95;,
 3;114,58,98;,
 3;51,98,50;,
 3;94,96,114;,
 3;96,95,114;,
 3;9,12,0;,
 3;10,3,29;,
 3;9,29,12;,
 3;29,9,10;,
 3;3,5,70;,
 3;118,41,43;,
 3;29,13,12;,
 3;17,11,14;,
 3;13,22,17;,
 3;41,31,33;,
 3;71,121,122;,
 3;73,37,71;,
 3;67,123,34;,
 3;43,124,119;,
 3;43,34,124;,
 3;20,72,81;,
 3;81,79,25;,
 3;47,49,69;,
 3;76,91,80;,
 3;73,122,76;,
 3;83,65,90;,
 3;49,63,69;,
 3;52,115,53;,
 3;105,104,107;,
 3;107,111,109;,
 3;111,110,109;;
 
 MeshMaterialList {
  1;
  214;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\stone004.jpg";
   }
  }
 }
 MeshNormals {
  110;
  0.593447;0.801679;-0.071642;,
  -0.270443;0.489877;-0.828783;,
  -0.392796;0.416090;0.820110;,
  -0.665782;0.599625;-0.444054;,
  -0.017712;-0.984260;-0.175838;,
  -0.228937;-0.890492;-0.393207;,
  0.123351;0.991757;0.034670;,
  -0.070915;0.831932;0.550328;,
  -0.187289;0.926663;-0.325912;,
  0.388479;0.705413;0.592855;,
  -0.647266;0.638472;0.416413;,
  -0.386098;0.241326;0.890332;,
  -0.885147;0.125460;0.448079;,
  0.733792;0.192669;-0.651481;,
  -0.257592;0.944848;0.202258;,
  -0.404658;0.889406;-0.212623;,
  0.586613;0.647020;-0.487084;,
  0.032356;0.845628;-0.532792;,
  0.665523;0.683683;-0.299427;,
  -0.259694;0.669160;0.696264;,
  -0.393628;0.791998;0.466687;,
  -0.549255;0.835635;0.005808;,
  0.295179;0.479364;-0.826486;,
  -0.322122;-0.078278;-0.943457;,
  -0.369972;-0.083360;-0.925295;,
  0.990730;-0.018633;-0.134563;,
  -0.482315;0.732387;-0.480605;,
  -0.990347;-0.124871;0.060172;,
  -0.659679;0.727145;0.189958;,
  -0.644406;-0.043544;0.763442;,
  -0.660585;0.175218;0.730018;,
  -0.435387;0.202641;0.877140;,
  -0.089435;0.446531;0.890287;,
  -0.877180;0.453875;-0.156694;,
  -0.687973;-0.450624;0.568886;,
  -0.877419;-0.358631;0.318622;,
  -0.878172;-0.214703;0.427453;,
  -0.474519;-0.101164;0.874413;,
  -0.820437;-0.102326;0.562506;,
  -0.958455;-0.167081;-0.231187;,
  -0.582641;-0.784782;0.211295;,
  -0.782712;-0.614389;0.099437;,
  -0.918280;-0.391300;0.060386;,
  -0.279196;-0.410507;-0.868063;,
  -0.298772;-0.535220;-0.790111;,
  -0.673204;0.347321;-0.652814;,
  -0.754691;0.011409;-0.655981;,
  -0.626853;-0.489540;-0.606140;,
  0.615834;-0.351583;-0.705080;,
  0.072231;-0.478790;-0.874953;,
  0.913975;-0.375999;0.152561;,
  0.650336;0.483049;-0.586282;,
  -0.231265;0.092567;-0.968477;,
  0.247732;0.193849;-0.949237;,
  0.236809;-0.088703;-0.967499;,
  -0.205545;-0.223630;-0.952754;,
  0.842676;0.426656;-0.328423;,
  -0.416748;0.355851;-0.836476;,
  -0.258061;0.807151;-0.530954;,
  0.698116;0.588264;-0.408140;,
  0.712013;0.627936;-0.314218;,
  0.948404;-0.017897;0.316558;,
  0.751575;0.658472;-0.039357;,
  0.851960;0.510192;0.117767;,
  0.694388;0.608728;0.383765;,
  0.751329;0.311774;0.581637;,
  0.905810;-0.032323;-0.422449;,
  0.473176;-0.596183;-0.648591;,
  -0.158018;-0.712397;-0.683755;,
  -0.310657;-0.715432;-0.625819;,
  -0.300255;-0.366374;-0.880692;,
  -0.880387;0.182547;-0.437716;,
  -0.525417;0.847176;-0.078931;,
  -0.412016;-0.653347;-0.635122;,
  -0.725277;-0.637441;-0.260080;,
  -0.275292;-0.960189;0.047451;,
  -0.541183;-0.804753;-0.243911;,
  0.180490;0.902737;-0.390498;,
  -0.030164;-0.839789;-0.542075;,
  0.417154;-0.864857;0.279293;,
  0.145266;-0.988156;-0.049453;,
  0.967550;-0.199661;-0.154863;,
  0.722086;-0.621600;0.303653;,
  0.556451;-0.830670;-0.018698;,
  0.434566;-0.632672;0.640998;,
  0.568000;0.481394;0.667559;,
  0.431732;0.829571;0.354145;,
  -0.103905;0.889383;0.445199;,
  0.514256;0.066921;0.855022;,
  0.340925;0.238620;0.909302;,
  0.142512;0.025905;0.989454;,
  0.593300;0.108809;0.797594;,
  0.829809;-0.429593;0.356183;,
  0.765657;-0.349887;0.539767;,
  0.484769;-0.198116;0.851909;,
  -0.091016;-0.449541;0.888610;,
  0.746316;0.014066;0.665443;,
  0.604684;-0.751822;0.262909;,
  -0.036746;-0.000421;0.999325;,
  0.035310;-0.866835;0.497344;,
  -0.275313;-0.142889;0.950676;,
  0.148302;-0.095279;0.984342;,
  -0.463841;-0.484021;0.742008;,
  -0.364323;-0.827306;0.427589;,
  0.149803;-0.969637;0.193296;,
  0.978301;0.198887;-0.058059;,
  0.614594;0.521781;-0.591624;,
  0.874799;-0.446049;-0.189121;,
  -0.242706;0.081630;0.966659;,
  0.301571;-0.451433;-0.839799;;
  214;
  3;19,87,14;,
  3;72,15,26;,
  3;58,8,77;,
  3;20,19,14;,
  3;20,14,21;,
  3;15,72,21;,
  3;26,15,58;,
  3;11,19,2;,
  3;2,30,31;,
  3;34,37,29;,
  3;29,31,30;,
  3;29,36,34;,
  3;36,35,34;,
  3;42,36,27;,
  3;11,2,31;,
  3;37,11,29;,
  3;29,38,36;,
  3;103,34,40;,
  3;40,34,41;,
  3;30,12,38;,
  3;27,71,39;,
  3;42,27,39;,
  3;10,72,28;,
  3;30,10,12;,
  3;36,42,35;,
  3;53,55,52;,
  3;52,55,23;,
  3;45,26,57;,
  3;24,46,45;,
  3;54,55,53;,
  3;51,22,16;,
  3;26,58,57;,
  3;52,23,1;,
  3;51,16,59;,
  3;56,59,60;,
  3;13,106,66;,
  3;60,59,62;,
  3;0,77,6;,
  3;25,56,105;,
  3;66,56,25;,
  3;65,63,64;,
  3;18,62,59;,
  3;25,105,61;,
  3;81,50,107;,
  3;50,92,82;,
  3;55,49,43;,
  3;54,53,106;,
  3;43,49,68;,
  3;49,55,54;,
  3;48,49,54;,
  3;15,8,58;,
  3;26,45,3;,
  3;24,57,23;,
  3;57,24,45;,
  3;47,46,24;,
  3;3,45,71;,
  3;70,47,24;,
  3;36,38,12;,
  3;10,28,12;,
  3;27,36,12;,
  3;12,33,27;,
  3;12,28,33;,
  3;3,33,72;,
  3;72,33,28;,
  3;47,70,73;,
  3;73,70,44;,
  3;5,75,76;,
  3;69,5,76;,
  3;47,73,74;,
  3;75,40,76;,
  3;71,45,46;,
  3;41,42,74;,
  3;35,42,41;,
  3;76,74,73;,
  3;3,71,33;,
  3;27,33,71;,
  3;46,39,71;,
  3;69,73,44;,
  3;47,39,46;,
  3;69,76,73;,
  3;41,34,35;,
  3;47,42,39;,
  3;41,76,40;,
  3;53,22,51;,
  3;59,16,18;,
  3;22,52,1;,
  3;51,106,53;,
  3;54,106,13;,
  3;48,54,109;,
  3;66,25,81;,
  3;67,83,78;,
  3;68,44,43;,
  3;49,78,68;,
  3;68,5,69;,
  3;4,68,78;,
  3;4,78,80;,
  3;83,79,80;,
  3;79,4,80;,
  3;25,50,81;,
  3;82,107,50;,
  3;5,68,4;,
  3;80,78,83;,
  3;104,4,79;,
  3;14,6,8;,
  3;87,19,7;,
  3;0,18,77;,
  3;14,15,21;,
  3;85,32,89;,
  3;63,62,64;,
  3;14,87,6;,
  3;64,62,86;,
  3;86,6,87;,
  3;91,85,89;,
  3;85,91,65;,
  3;32,90,89;,
  3;65,91,96;,
  3;96,93,92;,
  3;60,62,63;,
  3;96,91,88;,
  3;85,65,64;,
  3;101,88,90;,
  3;88,101,94;,
  3;96,88,93;,
  3;82,92,97;,
  3;84,93,94;,
  3;25,61,50;,
  3;96,92,61;,
  3;94,93,88;,
  3;92,50,61;,
  3;91,90,88;,
  3;90,98,101;,
  3;84,99,97;,
  3;99,104,97;,
  3;97,92,93;,
  3;102,95,100;,
  3;95,101,100;,
  3;98,100,101;,
  3;98,90,108;,
  3;100,37,102;,
  3;97,79,82;,
  3;89,90,91;,
  3;97,93,84;,
  3;104,99,103;,
  3;95,84,101;,
  3;37,100,108;,
  3;103,99,102;,
  3;97,104,79;,
  3;34,102,37;,
  3;40,75,103;,
  3;102,34,103;,
  3;11,37,108;,
  3;103,75,104;,
  3;96,61,65;,
  3;63,61,105;,
  3;61,63,65;,
  3;105,56,63;,
  3;56,60,63;,
  3;66,81,107;,
  3;59,56,106;,
  3;83,67,107;,
  3;106,51,59;,
  3;106,56,66;,
  3;82,83,107;,
  3;49,48,67;,
  3;67,48,107;,
  3;78,49,67;,
  3;75,5,4;,
  3;75,4,104;,
  3;32,19,11;,
  3;90,32,108;,
  3;108,32,11;,
  3;32,7,19;,
  3;86,87,7;,
  3;7,9,86;,
  3;108,100,98;,
  3;0,6,86;,
  3;8,15,14;,
  3;17,16,22;,
  3;77,16,17;,
  3;16,77,18;,
  3;1,58,17;,
  3;58,77,17;,
  3;0,62,18;,
  3;64,9,85;,
  3;9,64,86;,
  3;0,86,62;,
  3;7,32,9;,
  3;32,85,9;,
  3;20,2,19;,
  3;21,72,10;,
  3;20,10,2;,
  3;10,20,21;,
  3;72,26,3;,
  3;17,22,1;,
  3;10,30,2;,
  3;29,11,31;,
  3;30,38,29;,
  3;22,53,52;,
  3;23,55,43;,
  3;70,24,23;,
  3;43,70,23;,
  3;1,57,58;,
  3;1,23,57;,
  3;42,47,74;,
  3;74,76,41;,
  3;13,66,48;,
  3;44,68,69;,
  3;70,43,44;,
  3;83,82,79;,
  3;66,107,48;,
  3;77,8,6;,
  3;94,101,84;,
  3;84,95,99;,
  3;95,102,99;;
 }
 MeshTextureCoords {
  125;
  0.526190;0.195770;,
  0.427310;0.086780;,
  0.590230;0.063260;,
  0.775730;0.181850;,
  0.851660;0.096710;,
  0.890520;0.219650;,
  0.966100;0.222990;,
  1.025880;0.071210;,
  1.102220;0.159530;,
  0.618530;0.162120;,
  0.712320;0.122940;,
  0.531760;0.361550;,
  0.600230;0.247110;,
  0.639030;0.342630;,
  0.585850;0.353910;,
  0.622500;0.665430;,
  0.558340;0.520100;,
  0.609620;0.477120;,
  0.688190;0.522200;,
  0.695540;0.637010;,
  0.766440;0.569030;,
  0.750910;0.402940;,
  0.682860;0.402790;,
  0.551530;0.806440;,
  0.670000;0.803890;,
  0.735750;0.711160;,
  0.719480;0.347470;,
  0.812400;0.391180;,
  0.804790;0.491980;,
  0.661170;0.246580;,
  0.731970;0.272580;,
  0.061530;0.410840;,
  0.026300;0.507130;,
  0.020840;0.378890;,
  -0.020200;0.402450;,
  0.883440;0.348160;,
  0.937750;0.339220;,
  0.918310;0.423030;,
  0.865020;0.484180;,
  0.080420;0.483010;,
  0.109010;0.338100;,
  0.057500;0.316450;,
  0.110110;0.245400;,
  0.005290;0.286300;,
  0.153910;0.325650;,
  0.185550;0.412150;,
  0.207030;0.343120;,
  0.124700;0.476000;,
  0.131100;0.412800;,
  0.166390;0.507160;,
  0.254060;0.252660;,
  0.248530;0.168840;,
  0.102220;0.159530;,
  0.260900;0.078350;,
  0.220270;0.472360;,
  0.254810;0.432380;,
  0.329610;0.391690;,
  0.269370;0.370030;,
  0.326490;0.298210;,
  0.170950;0.212220;,
  0.284470;0.464760;,
  0.199830;0.562270;,
  0.247820;0.556960;,
  0.194780;0.642280;,
  0.310630;0.543840;,
  0.271460;0.657820;,
  0.091430;0.599840;,
  0.008700;0.588180;,
  0.029340;0.738440;,
  0.136100;0.572950;,
  0.825440;0.263520;,
  0.979800;0.402450;,
  0.863670;0.574800;,
  0.935310;0.565510;,
  0.770320;0.321790;,
  0.888100;0.705900;,
  0.952850;0.669210;,
  0.935240;0.885080;,
  0.689180;0.934000;,
  0.818490;0.811180;,
  0.959040;0.786210;,
  0.824810;0.673550;,
  0.141740;0.680230;,
  0.189320;0.718420;,
  0.113440;0.756860;,
  -0.047150;0.669210;,
  -0.064760;0.885080;,
  -0.040960;0.786210;,
  0.129950;0.891640;,
  0.189210;0.790900;,
  0.266870;0.799780;,
  1.029340;0.738440;,
  1.129950;0.891640;,
  0.383420;0.866410;,
  0.447780;0.177900;,
  0.387440;0.308160;,
  0.459200;0.295720;,
  0.427870;0.355240;,
  0.342420;0.158400;,
  0.379740;0.425350;,
  0.437450;0.444630;,
  0.336320;0.464110;,
  0.355850;0.581670;,
  0.388470;0.512270;,
  0.438930;0.588350;,
  0.395220;0.590870;,
  0.342320;0.698100;,
  0.411020;0.664800;,
  0.466340;0.517410;,
  0.455250;0.759700;,
  0.536720;0.693310;,
  0.465300;0.669190;,
  0.495430;0.587400;,
  0.500870;0.458970;,
  0.399690;0.219650;,
  0.025880;0.071210;,
  -0.148340;0.096710;,
  -0.409770;0.063260;,
  0.061140;0.234400;,
  -0.033900;0.222990;,
  1.061140;0.234400;,
  1.026300;0.507130;,
  1.008700;0.588180;,
  -0.064690;0.565510;,
  -0.062250;0.339220;;
 }
}
