META
{
  Parser = PFX;
};

instance ZWATERRING_TINY(C_PARTICLEFX)
{
  ppsvalue = 1;
  ppsislooping = 0;
  dirmode_s = "RAND";
  lsppartavg = 1200;
  lsppartvar = 200;
  flycolldet_b = 0;
  visname_s = "WATERRING.TGA";
  visorientation_s = "VOB";
  vistexisquadpoly = 1;
  vistexanifps = 0;
  vistexaniislooping = 0;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "220 240 255";
  vissizestart_s = "3 3";
  vissizeendscale = 5.0;
  visalphafunc_s = "ADD";
  visalphastart = 180;
  visalphaend = 0;
};

instance ZWATERRING_SMALL(C_PARTICLEFX)
{
  ppsvalue = 1;
  ppsislooping = 0;
  dirmode_s = "RAND";
  lsppartavg = 1800;
  lsppartvar = 300;
  flycolldet_b = 0;
  visname_s = "WATERRING.TGA";
  visorientation_s = "VOB";
  vistexisquadpoly = 1;
  vistexanifps = 0;
  vistexaniislooping = 0;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "220 240 255";
  vissizestart_s = "6 6";
  vissizeendscale = 6.0;
  visalphafunc_s = "ADD";
  visalphastart = 200;
  visalphaend = 0;
};

instance ZWATERRING_MEDIUM(C_PARTICLEFX)
{
  ppsvalue = 1;
  ppsislooping = 0;
  dirmode_s = "RAND";
  lsppartavg = 2500;
  lsppartvar = 500;
  flycolldet_b = 0;
  visname_s = "WATERRING.TGA";
  visorientation_s = "VOB";
  vistexisquadpoly = 1;
  vistexanifps = 0;
  vistexaniislooping = 0;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "220 240 255";
  vissizestart_s = "7 7";
  vissizeendscale = 6.5;
  visalphafunc_s = "ADD";
  visalphastart = 220;
  visalphaend = 0;
};

instance ZWATERRING_LARGE(C_PARTICLEFX)
{
  ppsvalue = 1;
  ppsislooping = 0;
  dirmode_s = "RAND";
  lsppartavg = 3500;
  lsppartvar = 800;
  flycolldet_b = 0;
  visname_s = "WATERRING.TGA";
  visorientation_s = "VOB";
  vistexisquadpoly = 1;
  vistexanifps = 0;
  vistexaniislooping = 0;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "220 240 255";
  vissizestart_s = "12 12";
  vissizeendscale = 8.0;
  visalphafunc_s = "ADD";
  visalphastart = 240;
  visalphaend = 0;
};

instance PFX_WATERSPLASH_TINY(C_PARTICLEFX)
{
  ppsvalue = 22;
  ppsscalekeys_s = "1";
  ppsissmooth = 1;
  ppsfps = 5;
  shptype_s = "CIRCLE";
  shpisvolume = 1;
  shpdim_s = "4 0 4";
  dirmode_s = "DIR";
  dirfor_s = "object";
  dirangleheadvar = 45;
  dirangleelev = 88;
  dirangleelevvar = 50;
  velavg = 0.14;
  velvar = 0.05;
  lsppartavg = 480;
  lsppartvar = 200;
  flygravity_s = "0 -0.0008 0";
  flycolldet_b = 0;
  visname_s = "WATERSPLASH1.TGA";
  visorientation_s = "VELO";
  vistexisquadpoly = 1;
  vistexaniislooping = 1;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "150 200 255";
  vissizestart_s = "2 2";
  vissizeendscale = 5;
  visalphafunc_s = "ADD";
  visalphastart = 210;
};

instance PFX_WATERSPLASH_SMALL(C_PARTICLEFX)
{
  ppsvalue = 25;
  ppsscalekeys_s = "1";
  ppsissmooth = 1;
  ppsfps = 5;
  shptype_s = "CIRCLE";
  shpisvolume = 1;
  shpdim_s = "5 0 5";
  dirmode_s = "DIR";
  dirfor_s = "object";
  dirangleheadvar = 45;
  dirangleelev = 88;
  dirangleelevvar = 50;
  velavg = 0.15;
  velvar = 0.05;
  lsppartavg = 500;
  lsppartvar = 250;
  flygravity_s = "0 -0.0008 0";
  flycolldet_b = 0;
  visname_s = "WATERSPLASH1.TGA";
  visorientation_s = "VELO";
  vistexisquadpoly = 1;
  vistexaniislooping = 1;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "150 200 255";
  vissizestart_s = "2 2";
  vissizeendscale = 5;
  visalphafunc_s = "ADD";
  visalphastart = 220;
};

instance PFX_WATERSPLASH_MEDIUM(C_PARTICLEFX)
{
  ppsvalue = 30;
  ppsscalekeys_s = "1";
  ppsissmooth = 1;
  ppsfps = 5;
  shptype_s = "CIRCLE";
  shpisvolume = 1;
  shpdim_s = "6 0 6";
  dirmode_s = "DIR";
  dirfor_s = "object";
  dirangleheadvar = 50;
  dirangleelev = 90;
  dirangleelevvar = 55;
  velavg = 0.16;
  velvar = 0.06;
  lsppartavg = 550;
  lsppartvar = 280;
  flygravity_s = "0 -0.0009 0";
  flycolldet_b = 0;
  visname_s = "WATERSPLASH1.TGA";
  visorientation_s = "VELO";
  vistexisquadpoly = 1;
  vistexaniislooping = 1;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "150 200 255";
  vissizestart_s = "2.5 2.5";
  vissizeendscale = 6;
  visalphafunc_s = "ADD";
  visalphastart = 230;
};

instance PFX_WATERSPLASH_LARGE(C_PARTICLEFX)
{
  ppsvalue = 45;
  ppsscalekeys_s = "1";
  ppsissmooth = 1;
  ppsfps = 5;
  shptype_s = "CIRCLE";
  shpisvolume = 1;
  shpdim_s = "12 0 12";
  dirmode_s = "DIR";
  dirfor_s = "object";
  dirangleheadvar = 55;
  dirangleelev = 90;
  dirangleelevvar = 65;
  velavg = 0.20;
  velvar = 0.08;
  lsppartavg = 650;
  lsppartvar = 350;
  flygravity_s = "0 -0.0010 0";
  flycolldet_b = 0;
  visname_s = "WATERSPLASH1.TGA";
  visorientation_s = "VELO";
  vistexisquadpoly = 1;
  vistexaniislooping = 1;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "150 200 255";
  vissizestart_s = "3.5 3.5";
  vissizeendscale = 8;
  visalphafunc_s = "ADD";
  visalphastart = 250;
};

instance PFX_WEAPONSPLASH(C_PARTICLEFX)
{
  ppsvalue = 20;
  ppsscalekeys_s = "1";
  ppsissmooth = 1;
  ppsfps = 5;
  shptype_s = "CIRCLE";
  shpisvolume = 1;
  shpdim_s = "5 0 5";
  dirmode_s = "DIR";
  dirfor_s = "object";
  dirangleheadvar = 45;
  dirangleelev = 90;
  dirangleelevvar = 60;
  velavg = 0.15;
  velvar = 0.05;
  lsppartavg = 500;
  lsppartvar = 300;
  flygravity_s = "0 -0.0008 0";
  flycolldet_b = 0;
  visname_s = "WATERSPLASH1.TGA";
  visorientation_s = "VELO";
  vistexisquadpoly = 1;
  vistexaniislooping = 1;
  vistexcolorstart_s = "255 255 255";
  vistexcolorend_s = "150 200 255";
  vissizestart_s = "2 2";
  vissizeendscale = 5;
  visalphafunc_s = "ADD";
  visalphastart = 200;
};
