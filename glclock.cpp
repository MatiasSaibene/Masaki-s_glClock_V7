
#include "glclock.H"


#ifdef WIN32
// IME �õ���
typedef BOOL (APIENTRY * PFNWINNLSENABLEIMEPROC)(HWND, BOOL) ;
#define WINNLSEnableIME		(*pWINNLSEnableIME)
#define USER32_DLL_NAME		"USER32.DLL"
static PFNWINNLSENABLEIMEPROC pWINNLSEnableIME = NULL ;
static HINSTANCE hLibUser32 = NULL ;

#endif	// #ifdef WIN32

static int WIDTH ;
static int HEIGHT ;

static String openGL32_DLL ;

// glReadBuffer ���Ѥ��ʤ��ե饰
static int DONOT_USE_GL_READ_BUFFER = FALSE ;

// �ǥ����ץ쥤�ꥹ�ȼ��̥ʥ��
static GLuint ROOF_LIST, ROOF_DETAILTEX_LIST, SHELL_LIST, SHELL_LIST_CULL, SHELL_DETAILTEX_LIST, SHELL_DETAILTEX_LIST_CULL, MIRROR_WALL_LIST,
	HEDRON_LIST, HEDRON_LIST2, HEDRON_LIST3, BOARD_LIST, BOARD_DETAILTEX_LIST, BOARD_DETAIL_LIST,
	HOUR_LIST, MINUTE_LIST, SECOND_LIST, SECOND_E3_LIST, MIRROR_MINUTE_LIST, MIRROR_SECOND_LIST,
	POINT_LIST, GLASS_LIST, M_LIST, GOLDEN_OBJECT_LIST,

	ENV_ROOF_LIST, ENV_ROOF_OVER_LIST[MAX_MOTION_N_SAMPLING], ENV_SHELL_LIST, ENV_SHELL_LIST_CULL, 
	ENV_MIRROR_WALL_LIST, ENV_BOARD_LIST, ENV_BOARD_DETAIL_LIST, ENV_GLASS_LIST,
	ENV_HOUR_LIST, ENV_MINUTE_LIST, ENV_SECOND_LIST, ENV_MIRROR_HOUR_LIST, ENV_MIRROR_MINUTE_LIST, ENV_MIRROR_SECOND_LIST, ENV_GOLDEN_OBJECTS_LIST,

	MATERIAL_GOLD, MATERIAL_IRON, MATERIAL_SHELL_SOLID, MATERIAL_GLASS, MATERIAL_GLASS2,
	MATERIAL_GLASS4, MATERIAL_METAL_RED, MATERIAL_RED_GRAY, MATERIAL_METAL_BLUE, 
	MATERIAL_METAL_GREEN, MATERIAL_MAT_IRON, MATERIAL_MAT_GOLD,
	MATERIAL_MAT_METAL_GREEN, MATERIAL_MAT_METAL_BLUE,
	MATERIAL_SPECULAR,

	MATERIAL_MIRROR_RED_GRAY,
	MATERIAL_IRON_SPECULAR, MATERIAL_RED_SPECULAR, MATERIAL_BLUE_SPECULAR, MATERIAL_GREEN_SPECULAR, MATERIAL_GOLD_SPECULAR,

//	METAL_LIGHT_ATTRIBUTE,

	ENV_TEXTURE_LIST, ENV_GLASS_TEXTURE_LIST, SOLID_TEXTURE_LIST, SOLID2_TEXTURE_LIST,
	CHROME_TEXTURE_LIST, GOLD_TEXTURE_LIST, DETAIL_TEXTURE_LIST,

// for ARB_multitexture or SGIS_multitexture
	SPECULAR_MULTI_TEXTURE_LIST,

	CLOCK_LIST, SIGN_LIST ;

// �ƥ�������͡���
//static GLuint TEXTURE_NAME ;

#ifdef GLCLOCK_RETURN_LONGJMP
jmp_buf	glclock_env ;
#endif	// #ifndef GLCLOCK_RETURN_LONGJMP


// �ƥ�������͡���
#define N_TEXTURE_NAMES		12

static GLuint CHROME_TEXTURE_NAME, GOLD_TEXTURE_NAME, SPECULAR_TEXTURE_NAME, CHROME2_TEXTURE_NAME ; //, GOLD2_TEXTURE_NAME ;
static GLuint GREEN_TEXTURE_NAME, BLUE_TEXTURE_NAME, RED_TEXTURE_NAME, PINK_TEXTURE_NAME ;
static GLuint SOLID_TEXTURE_NAME, SOLID2_TEXTURE_NAME, DETAIL_TEXTURE_NAME ;

static GLuint SPECULAR_MULTI_TEXTURE_NAME ;


static double	fps, avg_ktris, kTPS, sum_fps, max_fps, max_ktps ;

// static float sleepRatio ;

static GLfloat	scr_w, scr_h, spin_x, spin_y, dist_y, z_step, p_step, yp_step, 
				m_speed, t_ratio, i_ratio, t_max, p_min, p_max, ir_ratio, norm_ratio,
				fog_density, fog_start, fog_end, t_roof, near_clip, far_clip,
				shutter, s_shutter, dof_range, focus_distance, specular, spec,
				s_scale, t_scale, dtex_scale, tex_rotate, radius_ratio, n_width,
				screen_size, glclock_mark, detail_texture_bright, detail_texture_correct_bright ;

static GLint	sum_polygons, r_polygons, DragL, DragL_t, DragR, xsize, ysize, fog_mode, f_cnt, f_polygons, f_time, f_prn, f_cal_flag,
				clock_quality, clock_hedron, clock_glass, clock_div, explicit_clock_div, clock_point, clock_mesh, gold_depth,
				anti, default_anti, motion, s_motion, default_motion, pre_motion, pre_s_motion, lid, field, default_field,
				stop, start, nm_polygons, ngrad, n_golden, no_sign, no_message,
				bg_color, grad_flag, dither, light, local, texture, solid, crystal, reality, detail, full, reflection, // max_tex,
				benchmark, filterTest, rotateLight, testCount, firstFPS, startElapsedTime, maxTriTest, focusTest, mipmapTest,
				defaultTextureSize, orgImageFlag, firstError ;

static int		last_x, last_y, sign_x, sign_y, fps_x, fps_y, mouse_x, mouse_y, tex_size, u_smooth, v_smooth, g_smooth,
				master_fps_count, fps_count, dst_fps_count, env, loop_count = 0, is_set_width, is_set_height, m_sleep,
				wait_calc_fps, is_set_fog, fogFlag, multi_super_sample, changeTitleWait ;

static float	master_spin_ratio ;	// �٥���ޡ����κݤβ�ž®�ٴ��

static unsigned int cull_status ;
static unsigned int timerResolution ;

static GLenum	gl_min_filter, gl_mag_filter, gl_pol_mode, gl_pers_corr, gl_fog_hint ;
static String	textureFilter ;

static unsigned int glut_buff, glut_accum, glut_alpha, us_sleep ;
static String	env_file, light_file, solid_file, sld2_file, detail_file, s_fps, t_fps, fout_fps, fout_gl, image_path, env_path ;
static String	glVendor, glRenderer, glVersion, glExtensions ;
static String	glInformationsLog, glClientInformationsLog, glServerInformationsLog, colorBufferLog, texInformationLog ;
static String	f_FPS, f_FPS_kTPS, f_FPS_kTPS_kTPF, d_FPS_kTPS_kTPF ;
static String	tex_format ;
static int		isSet_texFormat ;

static GLint	tex_internal_format ;
static String	internalFormatSymbol, internalFormatInfos ;

static String	pre_fps_message ;
static String	sign, title, tmpTitle ;

static Vector2d *jitter = NULL ;
static Vector2d *jitter_dof = NULL ;
static TimeData	timeData ;
static ClockState now, pre ;

static TRUEIMAGE *solid_image = NULL, *env_image = NULL, *env2_image = NULL, *solid2_image = NULL, *light_image = NULL ;
static TRUEIMAGE *detail_image = NULL, *create_image = NULL ;
static TRUEIMAGE *specularImage = NULL, *multiTexSpecularImage = NULL ;

// ���դ���°���ѥƥ�������
static TRUEIMAGE *metalic_image = NULL, *gold_image = NULL; //, *gold2_image = NULL ;
static TRUEIMAGE *green_image = NULL, *blue_image = NULL, *red_image = NULL, *pink_image = NULL ;


//static Vector3d s(1.0f, 0.0f, 1.0f), l(0.0f, 1.0f, 0.0f), t, ms, mt ;
static const Vector3d csAxis(1.0f, 0.0f, -1.0f), ctAxis(0.0f, -1.0f, 1.0f) ;
static const Vector3d csAxis_orgImage(1.0f, 0.0f, 0.0f), ctAxis_orgImage(0.0f, -1.0f, 0.0f) ;
//static const Vector3d cdsAxis(1.0f, 0.0f, 0.0f), cdtAxis(0.0f, 1.0f, 0.0f) ;
static const Vector3d cdsAxis(1.0f, 0.0f, 1.0f), cdtAxis(0.0f, 1.0f, -1.0f) ;
static Vector3d sAxis, tAxis, dsAxis, dtAxis, ms, mt ;
static float *vdsAxis, *vdtAxis ;

static int		envOpenGL, voodoo = FALSE, mesa = FALSE, fxOpenGL = FALSE, drawCursor = FALSE ;
static int		saver, winIDglut = NONE, firstReshapeTime = -1 ;

static int		isSet_zOffset ;

static float	OpenGL_Version, clockSize = 1.0f, zOffset ;
static int		scissorSize, viewportX, viewportY, viewportVX, viewportVY, polygon_offset = FALSE ;
static int		donotUSe_AlphaGouraud, donotUseBlend_AddSmooth, donotUseBlend_Multiply, detailBlend, specularBlend ;
static int		interrupt_OpenGLError, donotCheck_OpenGLError ;
static int		use_OpenGL_1_0, use_OpenGL_1_1 ;
static int		donotuse_OpenGL_1_1, donotuse_OpenGL_1_2, donotUse_AutoSelectOpenGLEnvironmentMode ;
static int		donotUse_extensions ;
static int		donotUse_multitexture, donotUse_ARB_multitexture, donotUse_EXT_multitexture, donotUse_SGIS_multitexture ;
static int		donotUse_separate_specular_color, donotUse_ARB_separate_specular_color, donotUse_EXT_separate_specular_color ;
static int		donotUse_texture_filter_anisotropic, donotUse_EXT_texture_filter_anisotropic ;
static int		donotUse_texture_object, donotUse_EXT_texture_object ;
static int		donotUse_polygon_offset, donotUse_EXT_polygon_offset ;
static int		donotUse_texgen_reflection, donotUse_ARB_texture_cube_map, donotUse_EXT_texture_cube_map, donotUse_NV_texgen_reflection ;
static int		donotUse_texture_compression, donotUse_ARB_texture_compression ;
static int		donotUse_texture_compression_s3tc, donotUse_EXT_texture_compression_s3tc ;
static int		donotUse_texture_compression_FXT1, donotUse_3DFX_texture_compression_FXT1 ;
static int		donotUse_accum_buffer ;
static int		fakeSphereMap ;
static int		cacheImages ;
static int		conserveTex ;

static float	specularTimeGradRatio ;

static float	specularMapReflectGamma, detailMapGamma ;

#ifdef WIN32
static int		cw = 0, ch = 0 ;
static int		isChange_DisplaySettings ;
#endif

static int		lightType ;

static GLint	maxTextureSize ;
static GLfloat	maxAnisotropy, maxTextureMaxAnisotropy, default_maxAnisotropy ;

// �û�������Υ������ե�������GL_ONE_MINUS_DST_COLOR or GL_ONE��
static GLenum	addSrcBlendFactor, addDstBlendFactor, addSrcBlendFactorGlass, addDstBlendFactorGlass ;
static GLenum	multiSrcBlendFactor, multiDstBlendFactor ;
static float	envGlassBright, realitySpecularBright ;

static GLfloat	ambLight, bright, masterBright, light_vividness, specularFlag ;
static int		vertexShadingFlag ;

static float	solid_saturation ;
static float	solid_contrast ;

static float	spinSpeed ;

//static int		has_ARB_multitexture, has_SGIS_multitexture ;
static char		*has_multitexture ;
static String	multitexture_name ;
static int		maxTextureUnits ;
static int		limit_maxTextureUnits ;

static char		*has_texture_object ;
static String	texture_object_name ;

static char		*has_polygon_offset ;
static String	polygon_offset_name ;

static char		*has_separate_specular_color ;
static String	separate_specular_color_name ;

static char		*has_texture_filter_anisotropic ;
static String	texture_filter_anisotropic_name ;

static char		*has_texgen_reflection ;
static String	texgen_reflection_name ;

static char		*has_texture_compression_Enum ;
static String	texture_compression_Enum_name ;

static char		*has_texture_compression_s3tc ;
static String	texture_compression_s3tc_name ;

static char		*has_texture_compression_FXT1 ;
static String	texture_compression_FXT1_name ;


static int		noCullObj, lightTwoSide, cullObj ;


// Keyboard status
static int		isSupported_glutKeyboardStatus ;
static int		keyL, keyR, keyU, keyD ;

// Game GLUT Supported
static int		glut_gameMode ;
static String	glut_gameModeString ;

static String	logFile ;

// ����⡼�ɼ��̻�
static int		mode_normal, mode_solid, mode_env, mode_crystal, mode_real ;


// Rendering Flags
static int		flag_withoutAccumBufferSuperSampling ;
static int		flag_additiveAlphaSuperSampling ;
static int		flag_glNormal ;
static int		flag_glassTexCoord ;
static int		flag_shadeTexCoord ;
static int		flag_shadeTexCoordOnlySolid ;
static int		flag_modulateTexCoord ;


// Win32 Mesa �Ǥ� glGenList() ���������֤���
//#if defined __WIN32__ && defined MESA
//static GLuint	cerealListNo ;
//#endif	// #if defined __WIN32__ && defined MESA

#ifdef WIN32

static HANDLE	g_hFileMap = NULL ;
static char		*g_pFps = NULL ;
static int		bpp ;

HINSTANCE		hInstanceGlClock ;
static HICON	hIcon ;
static int		iconNo ;
static HWND		hWin ;

static BOOL		isExistWINNLSEnableIME = FALSE ;

#endif


// ���ϥ��ȥ꡼��
static FILE		*outstream = NULL ;


// ���ǡ���
static GLfloat  currentBG[4], currentLC[4], currentCL[4], currentGC[4], currentSP[4] ;


// �������鸫�����פθ����ʣ������ǡ�
static GLfloat	zAxis ;

static Vector3d lightAngle[8] ;
static Vector3d lightAngleVelocity[8] ;

static Vector3d bgColor[] = { Vector3d( 0.0f, 0.4f, 1.0f ),
							  Vector3d( 0.7f, 0.2f, 0.1f ),
							  Vector3d( 0.0f, 0.0f, 0.2f ),
							  Vector3d( 0.0f, 0.4f, 1.0f ),
							  Vector3d( 0.0f, 0.0f, 0.0f ) },
				lightCl[] = { Vector3d( 1.0f, 1.0f, 1.0f ),
							  Vector3d( 1.5f, 0.7f, 0.5f ),
//							  Vector3d( 0.8f, 0.8f, 1.5f ) } ;
							  Vector3d( 0.8f, 0.8f, 1.0f ) } ;
//							  Vector3d( 0.83f, 0.83f, 1.0f ) } ;

static GLfloat ldir[][4] = { { -1.2f,  1.2f,  1.0f,  0.0f },
                             {  1.0f,  0.3f, -0.5f,  0.0f },
                             {  1.0f, -1.0f, -1.0f,  0.0f },
                             { -1.0f,  0.0f, -0.3f,  0.0f },
                             { -0.5f, -1.0f,  0.0f,  0.0f },

                             {  0.6f, -0.6f,  1.0f,  0.0f },	// �����ܰʹ��ɲ�
                             {  0.3f,  1.0f, -0.3f,  0.0f },
                             {  0.8f,  0.6f,  0.5f,  0.0f } },

			   lpos[8][4],
			   sdir[8][4],

			   lcol[8][4],
			   master_lcol[][4] = { { 1.0f, 1.0f, 1.0f, 1.0f },
									{ 1.0f, 0.7f, 0.7f, 1.0f },
									{ 0.5f, 0.6f, 0.5f, 1.0f },
									{ 0.6f, 0.4f, 0.6f, 1.0f },
									{ 0.6f, 0.6f, 0.9f, 1.0f },

									{ 0.20f, 0.20f, 0.15f, 1.0f },
									{ 0.15f, 0.20f, 0.20f, 1.0f },
									{ 0.25f, 0.25f, 0.25f, 1.0f } },	// �����ܰʹ��ɲ�

/*
			   lcol[][4] = { { 1.0f, 1.0f, 1.0f, 1.0f },
                             { 1.0f, 0.0f, 0.0f, 1.0f },
                             { 0.0f, 1.0f, 0.0f, 1.0f },
                             { 0.6f, 0.0f, 0.6f, 1.0f },
							 { 0.0f, 0.0f, 0.9f, 1.0f },

							 { 0.25f, 0.25f, 0.25f, 1.0f },	// �����ܰʹ��ɲ�
							 { 0.00f, 0.20f, 0.20f, 1.0f },
							 { 0.20f, 0.20f, 0.00f, 1.0f } },
*/
			   black[4] = { 0.0f, 0.0f, 0.0f, 1.0f },
			   white[4] = { 1.0f, 1.0f, 1.0f, 1.0f },
			   gold [4] = { 1.0f, 0.75f, 0.05f, 1.0f },
			   red_h[4] = { 1.0f, 0.1f, 0.1f, 1.0f } ;

//			   metal_light_attribute = { 4.0f, 4.0f, 4.0f, 1.0f } ;

static Vector3d *rgrad, *rgrad_motion, *rgrad_anti, *rgrad_field,
				rcol[4] = { Vector3d(1.0f, 1.0f, 0.0f),	// ����΢�̤Υ��顼
							Vector3d(1.0f, 0.0f, 1.0f),
							Vector3d(0.0f, 1.0f, 1.0f),
							Vector3d(0.7f, 0.7f, 0.7f) } ;

// �����ѻ����������ǡ���
static GLfloat n_v[][3] = { { 0.0f, 0.0f, 0.5f }, { 0.1f, 0.0f, 0.4f },
						     { 0.25f,0.0f, 0.3f }, { 0.5f, 0.0f, 0.2f },
						     { 0.75f,0.0f, 0.15f}, { 0.9f, 0.0f, 0.1f },
						     { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
						     { 0.0f, 5.0f, 0.0f } },

				p_v[][3] = { { 0.0f, 0.0f, 0.2f }, { 0.0f,-0.4f, 0.0f },
							 { 0.2f, 0.2f, 0.0f }, {-0.2f, 0.2f, 0.0f } },
				mw_v[][2] = { { 14.0f, 0.0f }, { 14.3f, 3.8f } },

				s_v[][2] = { { 14.0f, 0.0f }, { 14.3f, 4.8f }, { 14.5f, 5.0f }, { 15.2f,  4.8f }, { 15.7f, 4.2f },	// -e -r �λ� s_v[4] = (16.4f, 3.4f) ��
							 { 17.0f, 1.5f }, { 17.4f, 1.92f}, { 17.6f, 2.0f }, { 20.0f,  0.0f }, { 20.4f,-0.5f },	// -e -r �λ� s_v[8] = (19.0f, 1.2f) ��
							 { 20.5f,-1.2f }, { 20.3f,-1.86f}, { 20.1f,-2.0f }, { 19.0f, -2.2f }, { 18.7f,-2.6f },
							 { 18.1f,-3.0f }, { 17.0f,-3.0f }, { 17.0f,-3.4f }, { 16.86f,-3.6f }, { 13.0f,-4.0f },
							 {  0.0f,-3.8f } },
				s2_v[][2] = { { 14.0f, 0.0f }, { 14.5f, 5.0f }, { 15.2f, 4.8f }, { 17.0f, 1.5f }, { 17.6f, 2.1f },
							  { 20.4f,-0.5f }, { 20.5f,-2.0f }, { 19.0f,-2.2f }, { 18.5f,-2.8f }, { 17.0f,-3.0f },
							  { 17.0f,-3.6f }, { 13.0f,-4.0f }, {  0.0f,-3.8f } },

				r_v[][2] = { {  0.0f, 6.6f }, {  8.8f, 6.0f }, { 14.3f, 5.2f }, { 14.4f, 5.0f },
							 { 14.2f, 4.6f }, { 13.7f, 4.6f }, { 13.5f, 5.0f } },
				r2_v[][2] = { { 0.0f, 6.6f }, {  8.8f, 6.0f }, { 14.3f, 5.2f }, { 14.4f, 5.0f }, { 13.7f, 4.6f } },

				m_v[][3] = { {  0.0f, 0.0f, 0.0f }, { -2.0f, 3.0f, 0.0f }, { -5.0f,-2.0f, 0.0f },
							 { -2.0f, 1.0f, 0.0f }, { -2.0f, 2.0f, 1.0f }, { 2.0f, 1.0f, 0.0f },
							 {  5.0f,-2.0f, 0.0f }, {  2.0f, 3.0f, 0.0f }, { 2.0f, 2.0f, 1.0f } },

				g_v[][3] = { {  0.0f, 3.0f, 3.0f }, { -5.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 0.0f, 4.5f, -0.6f }, { 0.0f, 0.0f, 4.0f } },

//				glass_v[][2] = { { 0.0f, 2.0f }, { 7.0f, 1.68f }, { 10.36f, 1.35f }, { 12.5f, 0.9f }, { 13.6f, 0.45f }, { 14.4f, -0.2f } } ;	//
//				glass_v[][2] = { { 0.0f, 1.9f }, { 10.0f, 1.62f }, { 12.0f, 1.35f }, { 13.3f, 0.9f }, { 13.9f, 0.45f }, { 14.4f, -0.2f } } ;
//				glass_v[][2] = { { 0.0f, 1.9f }, { 7.0f, 1.8f }, { 10.0f, 1.62f }, { 12.0f, 1.35f }, { 13.3f, 0.9f }, { 14.4f, -0.2f } } ;		//
//				glass_v[][2] = { { 0.0f, 2.15f }, { 7.0f, 1.9f }, { 10.0f, 1.65f }, { 12.0f, 1.3f }, { 13.3f, 0.8f }, { 14.4f, -0.1f } } ;
				glass_v[][2] = { { 0.0f, 1.8f }, { 7.0f, 1.55f }, { 10.0f, 1.3f }, { 12.0f, 0.95f }, { 13.3f, 0.45f }, { 14.4f, -0.45f } } ;


static int s_a[][2] = { { 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
						{ 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 1 },
						{ 1, 0 }, { 1, 1 }, { 0, 0 }, { 0, 1 }, { 1, 1 },
						{ 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 0 }, { 0, 0 } },	// [4] & [7] �ϡ�-e, -r ���� (1, 1) �����ꤵ���
		   s2_a[][2] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 },
						 { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
/*
static int s_a[][2] = { { 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
						{ 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 1 },
						{ 1, 0 }, { 1, 1 }, { 0, 0 }, { 0, 1 }, { 1, 1 },
						{ 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 0 }, { 0, 0 } },
		   s2_a[][2] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 },
						 { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
*/
/*
		   s_a2[][2] = { { 0, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
						 { 0, 0 }, { 1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 1 },
						 { 1, 0 }, { 1, 1 }, { 0, 0 }, { 0, 1 }, { 1, 1 },
						 { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 0 }, { 0, 0 } },
		   s2_a2[][2] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 0 },
						  { 0, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
*/
		   r_a[][2] = { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 0 } },
		   r2_a[][2] = { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 0 } },

		   m_o[][3] = { { 0, 1, 4 }, { 1, 2, 4 }, { 2, 3, 4 }, { 3, 0, 4 },
						{ 0, 5, 8 }, { 5, 6, 8 }, { 6, 7, 8 }, { 7, 0, 8 } },

		   g_o[][3] = { { 0, 1, 4 }, { 0, 4, 2 }, { 0, 3, 1 }, { 0, 2, 3 } },

		   quality[8][6],
		   quality_master[][6] = { { 0,  6, 0, 0, 0, 0 }, { 0,  8, 1, 1, 0, 0 }, { 0, 16, 1, 1, 0, 1 }, { 1, 24, 1, 1, 1, 1 },
								   { 1, 32, 2, 1, 1, 2 }, { 1, 48, 3, 1, 1, 2 }, { 1, 64, 3, 1, 1, 2 }, { 1, 96, 3, 1, 1, 3 } },

		   n_s_vtx[] = { SHELL2_VTX, SHELL_VTX },
		   n_r_vtx[] = { ROOF2_VTX, ROOF_VTX } ;


// �٥���ޡ����ƥ��ȹ��ܡʥ饤�ƥ��󥰥ƥ��ȡ�
static String lightTestTitle[N_GLCLOCK_MARK_LIGHT_TEST] =
{
	"1 Directional Light.",
	"2 Directional Lights.",
	"4 Directional Lights.",
	"8 Directional Lights.",

	"1 Directional Light with Local View.",
	"2 Directional Lights with Local Viewer.",
	"4 Directional Lights with Local Viewer.",
	"8 Directional Lights with Local Viewer.",

	"1 Point Light.",
	"2 Point Lights.",
	"4 Point Lights.",
	"8 Point Lights.",

	"1 Point Light with Local Viewer.",
	"2 Point Lights with Local Viewer.",
	"4 Point Lights with Local Viewer.",
	"8 Point Lights with Local Viewer.",

	"1 Spot Light.",
	"2 Spot Lights.",
	"4 Spot Lights.",
	"8 Spot Lights.",

	"1 Spot Light with Local Viewer.",
	"2 Spot Lights with Local Viewer.",
	"4 Spot Lights with Local Viewer.",
	"8 Spot Lights with Local Viewer.",
} ;
/*
static String lightTestTitle[N_GLCLOCK_MARK_LIGHT_TEST] =
{
	"1 Directional Light with Directional View.",
	"2 Directional Lights with Directional View.",
	"4 Directional Lights with Directional View.",
	"8 Directional Lights with Directional View.",

	"1 Directional Light with Positional View.",
	"2 Directional Lights with Positional View.",
	"4 Directional Lights with Positional View.",
	"8 Directional Lights with Positional View.",

	"1 Point Light with Directional View.",
	"2 Point Lights with Directional View.",
	"4 Point Lights with Directional View.",
	"8 Point Lights with Directional View.",

	"1 Point Light with Positional View.",
	"2 Point Lights with Positional View.",
	"4 Point Lights with Positional View.",
	"8 Point Lights with Positional View.",

	"1 Spot Light with Directional View.",
	"2 Spot Lights with Directional View.",
	"4 Spot Lights with Directional View.",
	"8 Spot Lights with Directional View.",

	"1 Spot Light with Positional View.",
	"2 Spot Lights with Positional View.",
	"4 Spot Lights with Positional View.",
	"8 Spot Lights with Positional View.",
} ;
*/


// �٥���ޡ����ƥ��ȹ��ܡʥ������ǥ��󥰥ƥ�������ƥ��ȡ�
static String lightTexTestTitle[N_GLCLOCK_MARK_LIGHT_TEX_TEST] =
{
	"5 Directional Lights.",
	"5 Directional Lights with Local Viewer.",

	"5 Point Lights.",
	"5 Point Lights with Local Viewer.",

	"5 Spot Lights.",
	"5 Spot Lights with Local Viewer.",
} ;
/*
static String lightTexTestTitle[N_GLCLOCK_MARK_LIGHT_TEX_TEST] =
{
	"5 Directional Lights with Directional View.",
	"5 Directional Lights with Positional View.",

	"5 Point Lights with Directional View.",
	"5 Point Lights with Positional View.",

	"5 Spot Lights with Directional View.",
	"5 Spot Lights with Positional View.",
} ;
*/

// �٥���ޡ����ƥ��ȹ��ܡʥե����ƥ��ȡ�
static String fogTestTitle[N_GLCLOCK_MARK_FOG_TEST] =
{
	"Non-Fogging.",
	"Linear Equation Fogging.",
	"1 Exponential Equation Fogging.",
	"2 Exponential Equation Fogging.",
} ;


// �٥���ޡ����ƥ��ȹ��ܡʥե��륿��󥰥ƥ��ȡ�
static String filterTestTitle[N_GLCLOCK_MARK_FILTER_TEST] =
{
	"Nearest Point Sampling.",
	"Mip-Mapping.",
	"Bi-Linear Filtering.",
	"Bi-Linear Filtering with Mip-Mapping.",
	"Tri-Linear Filtering (Bi-Linear with Mip-Map Linear).",
} ;


// ��ž�����ѹ��� 
GLfloat aRotate[16] ;


#ifdef WIN32	// Windows �ʤ�
void u_sleep(unsigned long usecs)
{
    Sleep(usecs / 1000) ;
    return ;
}
#endif

#if !defined WIN32 && defined NO_SELECT && !defined NO_SLEEP	// WIN32 �ǤϤʤ� select ��Ȥ�ʤ����
void u_sleep(unsigned long usecs)
{
    usleep(usecs) ;
    return ;
}
#endif

#if !defined WIN32 && !defined NO_SELECT && !defined NO_SLEEP	// WIN32 �ǤϤʤ� select ��Ȥ����
void u_sleep(unsigned long usecs)
{
    struct timeval tv ;
    tv.tv_sec  = usecs / 1000000L ;
    tv.tv_usec = usecs % 1000000L ;

    select (0, 0, 0, 0, &tv) ;
    return ;
}
#endif

#if !defined WIN32 && defined NO_SLEEP	// WIN32 �ǤϤʤ� NO_SLEEP ���������Ƥ�����
void u_sleep(unsigned long usecs)
{
    return ;
}
#endif


// ��λ
void glclock_exit(int status)
{
#ifndef GLCLOCK_RETURN_LONGJMP
	exit(status) ;

#else
	if (status == EXIT_SUCCESS)
		longjmp(glclock_env, GLCLOCK_RET_LONGJMP_SUCCESS) ;
	else
		longjmp(glclock_env, GLCLOCK_RET_LONGJMP_FAILURE) ;
#endif
}


// �������˥󥰥��å�
void ___ForceIgnoreWarning()
{
	printf("OpenGL_Version: %3.1f\n", OpenGL_Version) ;
	printf("grad_flag: %d\n", grad_flag) ;
}


// ���Ū�ʥ�����ɥ������ȥ륻�å�
// ���ôְ��Ū�˥����ȥ�򥻥å�
// �٥���ޡ������ȡ��������Ȥ��飱���롼�װ����̵��
void SetTemporaryWindowTitle(const String& title, int wait = 3)
{
	if (benchmark || loop_count < 10)
		return ;

	changeTitleWait = wait ;
	glutSetWindowTitle(title) ;
}


// ������ɥ������ȥ륻�å�
// SetTemporaryWindowTitle()
// �����ξ�硢������ְʾ�ФäƤ��ʤ����̵��
void SetWindowTitle(const String& title)
{
	if (!changeTitleWait)
		glutSetWindowTitle(title) ;
}


// �����ѥ��ȥ꡼��˽��ս��ϡʥХåե��ե�å����ա�
inline int logprintf(const char* format, ...)
{
	// -NOMESSAGE ���ץ�������̵��
	if (no_message || !outstream)
		return 0 ;

	int r ;

	va_list args ;
	va_start(args, format) ;
	r = vfprintf(outstream, format, args) ;
	va_end(args) ;

	fflush(outstream) ;
	return r ;
}


// �����ѥ��ȥ꡼��˽��ս��ϡʥХåե��ե�å����ա�
// ���顼�ѡ�-NOMASSAGE �ˤ�ä���������ʤ���
inline int errprintf(const char* format, ...)
{
	if (!outstream)
		return 0 ;

	int r ;

	va_list args ;
	va_start(args, format) ;
	r = vfprintf(outstream, format, args) ;
	va_end(args) ;

	fflush(outstream) ;
	return r ;
}


// �����ѥ��ȥ꡼��˽���
inline int OutputLog(const char *log)
{
	if (no_message || !outstream)
		return 0 ;

	int ret = fprintf(outstream, log) ;
	fflush(outstream) ;
	return ret ;
}


float MeasureTimerResolution()
{
	int msec = GetMiliSeconds() ;
	int startTime = msec, last ;
	int count = 0 ;
	float interval = 0.0f ;

	while (msec - startTime < 5000 && startTime <= msec && count < 20)
	{
		count ++ ;
		last = msec ;

		while ((msec = GetMiliSeconds()) == last) ;
		interval += (float)(msec - last) ;	// �ºݤ��Ѳ���
	}

	if (startTime >= msec)
		return 0.0f ;

	interval /= count ;
	return interval ;
}

// �ߥ���ñ�̻��ּ�����ͭ���ʴֳ֤�¬��
float TimerResolution()
{
	float interval = MeasureTimerResolution() ;
	if (interval == 0.0f) interval = MeasureTimerResolution() ;

	return interval ;
}

void FreeImage(TRUEIMAGE **fimage)
{
	if (*fimage) { freeimage(*fimage) ; *fimage = NULL ; }
}

void FreeUsedImages()
{
	if ( solid_image) FreeImage(&solid_image) ;
	if (   env_image) FreeImage(&env_image) ;
	if (  env2_image) FreeImage(&env2_image) ;
	if ( light_image) FreeImage(&light_image) ;
	if (solid2_image) FreeImage(&solid2_image) ;
	if (detail_image) FreeImage(&detail_image) ;
	if (create_image) FreeImage(&create_image) ;

	if (specularImage) FreeImage(&specularImage) ;
	if (multiTexSpecularImage) FreeImage(&multiTexSpecularImage) ;

	if (metalic_image) FreeImage(&metalic_image) ;
	if (gold_image   ) FreeImage(&gold_image) ;
//	if (gold2_image  ) FreeImage(&gold2_image) ;
	if (green_image  ) FreeImage(&green_image) ;
	if (blue_image   ) FreeImage(&blue_image) ;
	if (red_image    ) FreeImage(&red_image) ;
	if (pink_image   ) FreeImage(&pink_image) ;
}



void UpdateOpenGLTextureFilter()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter) ;
}


void SetTextureFilter(String filter = "")
{
	if (filter != "")
		textureFilter = filter ;

	if (envOpenGL == WIN95_GDI_FIX)
	{
		if      ("NN" == textureFilter) { textureFilter = "NL" ; }
		else if ("LN" == textureFilter) { textureFilter = "LL" ; }
	}

	if ("N"  == textureFilter)
	{
		// �ݥ���ȥ���ץ��
		gl_min_filter = gl_mag_filter = GL_NEAREST ;
	}
	else if ("L"  == textureFilter)
	{
		// �Х���˥��ե��륿
		gl_min_filter = gl_mag_filter = GL_LINEAR ;
	}
	else if ("NN" == textureFilter)
	{
		// �ߥåץޥå�
		gl_min_filter = GL_NEAREST_MIPMAP_NEAREST ;
		gl_mag_filter = GL_NEAREST ;
	}
	else if ("LN" == textureFilter)
	{
		// �Х���˥��ܥߥåץޥå�
		gl_min_filter = GL_LINEAR_MIPMAP_NEAREST ;
		gl_mag_filter = GL_LINEAR ;
	}
	else if ("NL" == textureFilter)
	{
		// �ߥåץޥåץ�˥�
		gl_min_filter = GL_NEAREST_MIPMAP_LINEAR ;
		gl_mag_filter = GL_NEAREST ;
	}
	else
	{
		// �ȥ饤��˥��ե��륿
		gl_min_filter = GL_LINEAR_MIPMAP_LINEAR ;
		gl_mag_filter = GL_LINEAR ;
	}
}

String TextureFilteringLog()
{
	// �ƥ������㡦�ե��륿
	String texInfoLog = "Texture " ;

	switch (gl_min_filter)
	{
	case GL_NEAREST:
		texInfoLog += "nearest point sampling" ; break ;
	case GL_LINEAR:
		texInfoLog += "bi-linear filtering" ; break ;
	case GL_NEAREST_MIPMAP_NEAREST:
		texInfoLog += "nearest with mip-map nearest" ; break ;
	case GL_LINEAR_MIPMAP_NEAREST:
		texInfoLog += "bi-linear with mip-map nearest" ; break ;
	case GL_NEAREST_MIPMAP_LINEAR:
		texInfoLog += "nearest with mip-map linear" ; break ;
	case GL_LINEAR_MIPMAP_LINEAR:
		texInfoLog += "tri-linear filtering (bi-linear with mip-map linear)" ; break ;
	}

	return texInfoLog ;
}


// �ƥ�������ե��륿�򹹿���OpenGL �˥��åȤ���
// ���Ū�˥�����ɥ������ȥ���ѹ�
// ���٤ƤΥƥ��������Х���ɤ���������ѹ�����ɬ�פ���
void UpdateTextureFilter()
{
	SetTextureFilter(textureFilter) ;

	if (!has_texture_object || mode_solid)	// �ƥ������㡦���֥������Ȼ�����ǽ�⤷���� -solid ��
		UpdateOpenGLTextureFilter() ;
	else
	{
		// �ƥ������㡦���֥������Ȼ��Ѳ�ǽ�ξ��ϡ�
		// ���٤ƤΥƥ�������͡���˥Х���ɤ���������ѹ�
		for (int texNo = 0 ; texNo < TextureObjectCtrl::textureObjectCtrl.NTextures() ; texNo ++)
		{
			GLuint texName = TextureObjectCtrl::textureObjectCtrl[texNo] ;
			glBindTexture(GL_TEXTURE_2D, texName) ;
			UpdateOpenGLTextureFilter() ;
		}
	}

	SetTemporaryWindowTitle(TextureFilteringLog()) ;
}


// ��ǽ�Ǥ���С�Anisotropic Filtering ��ͭ����
void UpdateOpenGLAnisotropy(float anisotropy = -1.0f)
{
	if (maxTextureMaxAnisotropy <= 1.0f)
		return ;

	if (anisotropy < 0.0f)
		anisotropy = maxTextureMaxAnisotropy ;

	if (anisotropy < 1.0f)
		anisotropy = 1.0f ;

	// ��ǽ�Ǥ���С�Anisotropic Filtering ��ͭ����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy) ;
}


// ��ǽ�Ǥ���С�Anisotropic Filtering ��ͭ����
void UpdateAnisotropy(float anisotropy = -1.0f)
{
	if (maxTextureMaxAnisotropy <= 1.0f)
		return ;

	if (anisotropy < 0.0f)
		anisotropy = maxTextureMaxAnisotropy ;

	if (anisotropy < 1.0f)
		anisotropy = 1.0f ;

	if (!has_texture_object || mode_solid)	// �ƥ������㡦���֥������Ȼ�����ǽ�⤷���� -solid ��
		UpdateOpenGLAnisotropy(anisotropy) ;
	else
	{
		// ��ǽ�Ǥ���С�Anisotropic Filtering ��ͭ����
		// �ƥ������㡦���֥������Ȼ��Ѳ�ǽ�ξ��ϡ�
		// ���٤ƤΥƥ�������͡���˥Х���ɤ���������ѹ�
		for (int texNo = 0 ; texNo < TextureObjectCtrl::textureObjectCtrl.NTextures() ; texNo ++)
		{
			GLuint texName = TextureObjectCtrl::textureObjectCtrl[texNo] ;
			glBindTexture(GL_TEXTURE_2D, texName) ;
			UpdateOpenGLAnisotropy(anisotropy) ;
		}
	}
}


#ifdef WIN32
void SetDisplayBPP(int b = 0, int w = 0, int h = 0)
{
	// �ե륹���꡼��Ǥʤ����̵��
	if (!full) return ;

	// bpp ��Ŭ�ڤ��ͤ�
	if      (bpp <= 0) return ;	// 0 ���ѹ��ʤ�
//	else if (bpp <  4) b =  1 ;
//	else if (bpp <  8) b =  4 ;
	else if (bpp < 15) b =  8 ;
	else if (bpp < 24) b = 16 ;
	else if (bpp < 32) b = 24 ;
	else               b = 32 ;

	LONG ret ;
	DEVMODE dm ;
	dm.dmSize=sizeof(dm);
	dm.dmFields = DM_BITSPERPEL ;
	dm.dmBitsPerPel = (DWORD)bpp ;

	// w, h �������ʳ��ʤ顢�����٤��ѹ�
	if (w)
	{
		dm.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT ;
		dm.dmPelsWidth  = w ;
		dm.dmPelsHeight = h ;
	}

	ret = ChangeDisplaySettings(&dm, CDS_FULLSCREEN) ;

	// ��֡��Ȥ�ɬ�׻��ޤ����������ޤ��� 256�ʲ����ѹ��������
	if (ret == DISP_CHANGE_SUCCESSFUL || ret == DISP_CHANGE_RESTART || b <= 8)
		return ;

	// �ѹ��˼��Ԥ�����
	if (ret != DISP_CHANGE_SUCCESSFUL)
	{
		// �����⡼�ɤǺ��٥ƥ���
		if      (b = 15) dm.dmBitsPerPel = 16 ;
		else if (b = 16) dm.dmBitsPerPel = 15 ;
		else if (b = 24) dm.dmBitsPerPel = 32 ;
		else if (b = 32) dm.dmBitsPerPel = 24 ;

		ret = ChangeDisplaySettings(&dm, CDS_FULLSCREEN) ;
		if (ret != DISP_CHANGE_SUCCESSFUL)
		{
			// �����⡼�ɤǺ��٥ƥ���
			if (b == 15 || b == 16) dm.dmBitsPerPel = 24 ;
			else                    dm.dmBitsPerPel = 15 ;

			ret = ChangeDisplaySettings(&dm, CDS_FULLSCREEN) ;
			if (ret != DISP_CHANGE_SUCCESSFUL)
			{
				// �����⡼�ɤǺ��٥ƥ���
				if (dm.dmBitsPerPel == 15) dm.dmBitsPerPel = 16 ;
				else                       dm.dmBitsPerPel = 32 ;
				ChangeDisplaySettings(&dm, CDS_FULLSCREEN) ;
			}
		}
	}
}


// �����ѹ��������ɤ������֤�
int SetDisplayMode(int w, int h, int& cw, int& ch)
{
	static int scr[NM_RESOLUTION][2] =
	{
		{ 320,  200 }, {  320,  240 }, {  400,  300 }, {  512,  384 },
		{ 640,  400 }, {  640,  480 }, {  800,  600 }, { 1024,  768 },
		{ 1152, 864 }, { 1280, 1024 }, { 1600, 1200 }, { 2048, 1536 }
	} ;

	if (!is_set_width || !is_set_height || !full) return FALSE ;	// �ѹ��ʤ�

	// Voodoo�⡼�ɤξ���Ŭ�ڤ��ͤ�
	if (voodoo)
	{
		if      (w <  640) { w =  512 ; h = 384 ; }
		else if (w <  800) { w =  640 ; h = 480 ; }
		else if (w < 1024) { w =  800 ; h = 600 ; }
		else               { w = 1024 ; h = 768 ; }
	}

	DEVMODE dm ;
	dm.dmSize=sizeof(dm);
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT ;

	int d_mode ;
	if (voodoo) d_mode = 2 ; // scr[2 + 1] = { 512, 384 }
	else        d_mode = -1 ;

	if (w > scr[NM_RESOLUTION - 1][0] || h > scr[NM_RESOLUTION - 1][1])
		d_mode = NM_RESOLUTION - 1 - 1 ;
	else
	{
		while (scr[d_mode + 1][0] < w || scr[d_mode + 1][1] < h) d_mode ++ ;
	}

	int allFail = FALSE ;
	do
	{
		d_mode ++ ;
		if (d_mode >= NM_RESOLUTION)
		{
			allFail = TRUE ;
			break ;
		}

		dm.dmPelsWidth  = scr[d_mode][0] ;
		dm.dmPelsHeight = scr[d_mode][1] ;

	} while (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) ;

	// ���Ƽ��Ԥ�����ʤĤޤꡢ���������ͤ����礭�����꤬����Ƥ�������
	if (allFail)
	{
		allFail = FALSE ;
		do
		{
			d_mode -- ;
			if (d_mode > NM_RESOLUTION)
			{
				allFail = TRUE ;
				break ;
			}

			dm.dmPelsWidth  = scr[d_mode][0] ;
			dm.dmPelsHeight = scr[d_mode][1] ;

		} while (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) ;
	}

	int changeFlag ;

	if (!allFail)
	{
		// ������������ cw, ch �˼ºݤ��ѹ������������򥻥å�
		cw = dm.dmPelsWidth ;
		ch = dm.dmPelsHeight ;
		changeFlag = TRUE ;
	}
	else
	{
		// ���Ƽ��Ԥ������ʥ����ȤΤޤޡˤ� cw, ch �˥����򥻥å�
		cw = 0 ;
		ch = 0 ;
		changeFlag = FALSE ;
	}

	return changeFlag ;
}

void SetDefaultDisplayMode()
{
	if (isChange_DisplaySettings)
		ChangeDisplaySettings(NULL, CDS_FULLSCREEN) ;
}
#endif


// ��λ����
void EndProc()
{
	FreeUsedImages() ;

	if (jitter) DeleteSampleJitter(jitter) ;
	if (jitter_dof) DeleteSampleJitter(jitter_dof) ;

#ifdef IS_SUPPORTED_GLUT_GAME_MODE
	if (glut_gameMode)
	{
		glutLeaveGameMode() ;
	}
#endif

	if (!glut_gameMode)
	{
		if (winIDglut)
			glutDestroyWindow(winIDglut) ;
	}

	if (rgrad)			delete [] rgrad ;
	if (rgrad_motion)	delete [] rgrad_motion ;
	if (rgrad_anti)		delete [] rgrad_anti ;
	if (rgrad_field)	delete [] rgrad_field ;

#ifdef WIN32
	OpenGLDLLCtrl::openGLDLLCtrl.FreeLibrary() ;

	if (hIcon)
		DeleteObject(hIcon) ;

	if (g_pFps) UnmapViewOfFile(g_pFps) ;

#endif	// #ifdef WIN32

#if defined WIN32 // && defined VOODOO_MODE
	if (!glut_gameMode)
		SetDefaultDisplayMode() ;

	if (voodoo) UpdateWindow(HWND_BROADCAST) ;
//		PostMessage(HWND_BROADCAST, WM_PAINT, 0, 0) ;
//		SendMessage(HWND_BROADCAST, WM_PAINT, 0, 0) ;
#endif


#ifdef OVERRIDE_NEW_OPERATOR
	void MemDebugFinal() ;
	MemDebugFinal() ;
#endif	// #ifdef OVERRIDE_NEW_OPERATOR

	// �������ϥ��ȥ꡼�९������
	if (logFile == OUTPUT_LOG_FILE)
		fclose(outstream) ;
}


// �������ȥ��åץ�å�����
void StartupMessage()
{
	logprintf(String(TITLE_VER) + "\n" + AUTHOR + "\n\n") ; 
}


// ���᡼�����ԡ�
TRUEIMAGE *CopyImage(TRUEIMAGE *src)
{
	TRUEIMAGE *image ;
	image = CreateCopyImage(src) ;
	if (!image)
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Can't Create Copy Image."), _T("Createing Image error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't Create Copy Image.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}

	return image ;
}


// ���������᡼���������ΰ���ݤΤߡ�
TRUEIMAGE *NewImage(unsigned int w, unsigned int h = 0)
{
	TRUEIMAGE *image ;
	image = CreateNewImage(w, h) ;
	if (!image)
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Can't Create New Image."), _T("Createing Image error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't Create New Image.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}

	return image ;
}


// ����ˣ��ԥ��������ν������ͤ����ä����������᡼������
TRUEIMAGE *NewDefaultImage(unsigned int size)
{
	TRUEIMAGE *image ;
	image = NewImage(size) ;

	// ���ԥ��������ν������ͥ��å�
	ClearImage(image, 1.0f) ;
	unsigned int x, y ;
	y = size / 2 ;
	for (x = 0 ; x < size ; x ++)
	{
		unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R ���ǤΥݥ���
		ptr[0] = (unsigned char)(0) ;	// R
		ptr[1] = (unsigned char)(0) ;	// G
		ptr[2] = (unsigned char)(0) ;	// B
	}

	x = size / 2 ;
	for (y = 0 ; y < size ; y ++)
	{
		unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R ���ǤΥݥ���
		ptr[0] = (unsigned char)(0) ;	// R
		ptr[1] = (unsigned char)(0) ;	// G
		ptr[2] = (unsigned char)(0) ;	// B
	}

	return image ;
}


// �������ѹ��������᡼���򿷤��˺���
TRUEIMAGE *ScaledNewImage(TRUEIMAGE *src, unsigned int w, unsigned int h)
{
	TRUEIMAGE *image ;
	image = NewImage(w, h) ;

	int err ;
	if (err = gluScaleImage(GL_RGB, src->width, src->height,
							GL_UNSIGNED_BYTE, src->dataptr,
							image->width, image->height,
							GL_UNSIGNED_BYTE, image->dataptr)
		)
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		freeimage(image) ;
		glclock_exit(EXIT_FAILURE) ;
	}

	return image ;
}


void CheckOpenGLError()
{
	if (donotCheck_OpenGLError)
		return ;

	GLenum errCode = glGetError() ;

	if (errCode != GL_NO_ERROR)
	{
		String error ;
		switch (errCode)
		{
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM" ;
			break ;
		case GL_INVALID_VALUE:
			error = "GL_INVALID_VALUE" ;
			break ;
		case GL_INVALID_OPERATION:
			error = "GL_INVALID_OPERATION" ;
			break ;
		case GL_STACK_OVERFLOW:
			error = "GL_STACK_OVERFLOW" ;
			break ;
		case GL_STACK_UNDERFLOW:
			error = "GL_STACK_UNDERFLOW" ;
			break ;
		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY" ;
			break ;
		}
#ifdef WIN32
		if (interrupt_OpenGLError || firstError)
		{
			MessageBox(NULL, _T((char *)gluErrorString((GLenum)errCode)), "OpenGL Error: " + error, MB_OK | MB_ICONSTOP) ;
			firstError = FALSE ;
		}
#endif
		errprintf(String("OpenGL Error: ") + error + ": %s\n", (char *)gluErrorString((GLenum)errCode)) ;

		if (interrupt_OpenGLError)
		{
			EndProc() ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}
}


// clock_div ���פη��ˤ�äƴ���Ū��Ⱦ�¤���Ψ���Ѳ��ʣ��ѷ��ʾ�ʤ� 1.0f��
GLfloat RadiusRatio(int div)
{
	switch (div)
	{
	case 3:  return 0.55f ;
	case 4:  return 0.75f ;
	case 5:  return 0.86f ;
	case 6:  return 0.95f ;
	default: return 1.0f ;
	}
}


GLuint AllocAList(GLuint& list)
{
	// Win32 Mesa �Ǥϥ�����������ǽ��
//#if defined __WIN32__ && defined MESA
//	cerealListNo ++ ;
//	return cerealListNo ;
//
//#else
	if (!(list = glGenLists(1)))
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Can't allocate Display Lists"), _T("glclock error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't allocate a Display List.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}
	return list ;

//#endif	// #if defined __WIN32__ && defined MESA
}


// texture_object ����
GLuint AllocATexture(GLuint& texture)
{
	TextureObjectCtrl::textureObjectCtrl.AllocateATexture(texture) ;
	return texture ;
}
/*
GLuint AllocATexture(GLuint& texture)
{
	glGenTextures(1, &texture) ;
	return texture ;
}
*/

GLenum GetHintMode(String m)
{
	if (m == String("F")) return GL_FASTEST ;
	if (m == String("N")) return GL_NICEST ;
	else                  return GL_DONT_CARE ;
}


// ʸ���׾�ξ�����γ����
void DrawClockPoint(GLfloat x, GLfloat y, GLfloat z, float *s = NULL, float *t = NULL)
{
	GLfloat       m[] = {   x, 0.0f, 0.0f, 0.0f,
					       0.0f,   y, 0.0f, 0.0f,
					       0.0f, 0.0f,   z, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f } ;

	Object point(3) ;

	point.MakePolygon(0, 0,2,3, p_v, m) ;
	point.MakePolygon(1, 0,3,1, p_v, m) ;
	point.MakePolygon(2, 0,1,2, p_v, m) ;

	point.AverageNormal(0, 0, 0,1,2) ;
	point.AverageNormal(0, 1, 0,2) ;
	point.AverageNormal(0, 2, 0,1) ;
	point.AverageNormal(1, 0, 0,1,2) ;
	point.AverageNormal(1, 1, 0,1) ;
	point.AverageNormal(1, 2, 1,2) ;
	point.AverageNormal(2, 0, 0,1,2) ;
	point.AverageNormal(2, 1, 1,2) ;
	point.AverageNormal(2, 2, 0,2) ;

	glBegin(GL_TRIANGLE_FAN) ;

		if (flag_glNormal) glNormal3fv(point.polygon[0].normal[0].v) ;
		if (s && t)
		{
			Vector3d& v = point.polygon[0].vertex[0] ;
			TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
		}
		glVertex3fv(point.polygon[0].vertex[0].v) ;

		if (flag_glNormal) glNormal3fv(point.polygon[0].normal[1].v) ;
		if (s && t)
		{
			Vector3d& v = point.polygon[0].vertex[1] ;
			TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
		}
		glVertex3fv(point.polygon[0].vertex[1].v) ;

		if (flag_glNormal) glNormal3fv(point.polygon[1].normal[1].v) ;
		if (s && t)
		{
			Vector3d& v = point.polygon[1].vertex[1] ;
			TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
		}
		glVertex3fv(point.polygon[1].vertex[1].v) ;

		if (flag_glNormal) glNormal3fv(point.polygon[2].normal[1].v) ;
		if (s && t)
		{
			Vector3d& v = point.polygon[2].vertex[1] ;
			TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
		}
		glVertex3fv(point.polygon[2].vertex[1].v) ;

		if (flag_glNormal) glNormal3fv(point.polygon[0].normal[1].v) ;
		if (s && t)
		{
			Vector3d& v = point.polygon[0].vertex[1] ;
			TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
		}
		glVertex3fv(point.polygon[0].vertex[1].v) ;

	glEnd() ;
}

// ������
void DrawM(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, float *s = NULL, float *t = NULL)
{
	int    i ;
	Object objM(8) ;
	GLfloat m[] = {   x, 0.0f, 0.0f, 0.0f,
					 0.0f,   y, 0.0f, 0.0f,
					 0.0f, 0.0f,   z, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f } ;

	for (i = 0 ; i < M_VTX - 1 ; i ++) objM.MakePolygon(i, m_o[i][0],m_o[i][1],m_o[i][2], m_v, m) ;
	objM.DrawTriangles(FALSE, TRUE, FALSE, flag_glNormal, s, t) ;
}

// ʸ���׾�Σ����Ĥ�γ�Σ�������
void DrawPolyhedron(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, float *s = NULL, float *t = NULL)
{
	if (clock_hedron)
	{
//		Icosahedron obj(x,y,z, depth, average, smooth) ;
//		obj.Draw(NONE, NONE, TRUE) ;

		Icosahedron::DrawPolyhedron(x, flag_glNormal, s, t) ;
	}
	else
	{
//		Octahedron obj(x,y,z, depth, average, smooth) ;
//		obj.Draw(NONE, NONE, TRUE) ;

		Octahedron::DrawPolyhedron(x, flag_glNormal, s, t) ;
	}
}


// ʸ���׾�Σ����Ĥ�������
inline void DrawHedrons(GLfloat radius, GLfloat h_rotate)
{
	int	     i ;
	GLfloat rad ;

	glShadeModel(GL_FLAT) ;
	glCallList(MATERIAL_GOLD) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_LIGHTING_BIT) ;
					glCallList(MATERIAL_METAL_RED) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_LIGHTING_BIT) ;
					glCallList(MATERIAL_IRON) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


// ʸ���׾�Σ����Ĥ�������
// glColor �ǿ��������å�
inline void DrawHedronsWithColorMaterialDiffuse(GLfloat radius, GLfloat h_rotate)
{
	int	     i ;
	GLfloat rad ;

	glShadeModel(GL_FLAT) ;
	glColor3fv(gold) ;	// Diffuse Ambient �򥻥å�

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glColor3fv(red_h) ;	// Diffuse Ambient �򥻥å�
					glCallList(HEDRON_LIST2) ;
				glColor3fv(gold) ;	// Diffuse Ambient �򥻥åȡʲ������᤹��
			}
			else if (i == 6)
			{
				glColor3fv(white) ;	// Diffuse Ambient �򥻥å�
					glCallList(HEDRON_LIST3) ;
				glColor3fv(gold) ;	// Diffuse Ambient �򥻥åȡʲ������᤹��
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}

inline void DrawHedronsWithColorMaterialDiffuseTest(GLfloat radius, GLfloat h_rotate)
{
	int	     i ;
	GLfloat rad ;

	glShadeModel(GL_FLAT) ;
//	glColor3fv(gold) ;	// Diffuse Ambient �򥻥å�
//==================================

				glDisable(GL_COLOR_MATERIAL) ;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gold) ;


	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
//				glColor3fv(red_h) ;	// Diffuse Ambient �򥻥å�
//====================================
									glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red_h) ;
					glCallList(HEDRON_LIST2) ;
//				glColor3fv(gold) ;	// Diffuse Ambient �򥻥åȡʲ������᤹��
//====================================
									glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gold) ;
			}
			else if (i == 6)
			{
//				glColor3fv(white) ;	// Diffuse Ambient �򥻥å�
//====================================
									glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white) ;
					glCallList(HEDRON_LIST3) ;
//				glColor3fv(gold) ;	// Diffuse Ambient �򥻥åȡʲ������᤹��
//====================================
									glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gold) ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


// ���ѡ�ʸ���׾�Σ����Ĥ�������ʥ����Х���ץ���ѡ�
inline void DrawMirrorHedronsWithOverSampling(GLfloat radius, GLfloat h_rotate, GLfloat alpha_weight = 1.0f)
{
	int	     i ;
	GLfloat rad ;

//	glShadeModel(GL_FLAT) ;
	glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, alpha_weight) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0], currentCL[1]*0.1f, currentCL[2]*0.2f, alpha_weight) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0], currentCL[1], currentCL[2], alpha_weight) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


// �����ѹ���������˥ƥ���������ѹ�
//#ifdef GL_VERSION_1_1

// ʸ���׾�Σ����Ĥ�������
inline void DrawMirrorHedronsWithBindingTexture(GLfloat radius, GLfloat h_rotate, GLfloat blight_weight = 1.0f, GLfloat alpha = 0.4f)
{
	int	     i ;
	GLfloat rad ;

//	glShadeModel(GL_FLAT) ;
//	glColor4f(currentCL[0] * blight_weight, currentCL[1]*0.75f * blight_wait, currentCL[2]*0.05f * blight_wait, alpha) ;
	glColor4f(currentCL[0] * blight_weight, currentCL[1] * blight_weight, currentCL[2] * blight_weight, alpha) ;
	glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
//					glColor4f(currentCL[0] * blight_wait, currentCL[1]*0.1f * blight_wait, currentCL[2]*0.2f * blight_wait, alpha) ;
					glBindTexture(GL_TEXTURE_2D, RED_TEXTURE_NAME) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;

				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
//					glColor4f(currentCL[0] * blight_wait, currentCL[1] * blight_wait, currentCL[2] * blight_wait, alpha) ;
					glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;

				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}

//#endif	// #ifdef GL_VERSION_1_1


// ʸ���׾�Σ����Ĥ�������
// �Х���ɤȥޥƥꥢ�������ξ����¹�
inline void DrawHedronsWithBindingTextureAndSettingMaterial(GLfloat radius, GLfloat h_rotate)
{
	int	     i ;
	GLfloat rad ;

	glShadeModel(GL_FLAT) ;
	glCallList(MATERIAL_GOLD) ;
	glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glCallList(MATERIAL_METAL_RED) ;
					glBindTexture(GL_TEXTURE_2D, RED_TEXTURE_NAME) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;

				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glCallList(MATERIAL_IRON) ;
					glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;

				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


inline void DrawModulateHedrons(GLfloat radius, GLfloat h_rotate)
{
	int	     i ;
	GLfloat rad ;

//	if (mode_solid != 1 || (mode_normal /* && light == 0 */)) glShadeModel(GL_FLAT) ;
	glColor3f(currentCL[0], currentCL[1] * 0.75f, currentCL[2] * 0.05f) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor3f(currentCL[0], currentCL[1] * 0.1f, currentCL[2] * 0.2f) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor3fv(currentCL) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}

// ���ѡ�ʸ���׾�Σ����Ĥ�������
inline void DrawMirrorHedrons(GLfloat radius, GLfloat h_rotate, GLfloat blight_wait = 1.0f, GLfloat alpha = 0.4f)
{
	int	     i ;
	GLfloat rad ;

//	glShadeModel(GL_FLAT) ;
	glColor4f(currentCL[0] * blight_wait, currentCL[1]*0.75f * blight_wait, currentCL[2]*0.05f * blight_wait, alpha) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0] * blight_wait, currentCL[1]*0.1f * blight_wait, currentCL[2]*0.2f * blight_wait, alpha) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0] * blight_wait, currentCL[1] * blight_wait, currentCL[2] * blight_wait, alpha) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


// ���ꥹ����Σ����Ĥ�������
inline void DrawCrystalHedrons(GLfloat radius, GLfloat h_rotate, GLfloat color_weight = 1.0f)
{
	int	     i ;
	GLfloat rad ;

//	glShadeModel(GL_FLAT) ;
	glColor4f(currentCL[0] * color_weight, currentCL[1]*0.75f * color_weight, currentCL[2]*0.05f * color_weight, 0.4f) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0] * color_weight, currentCL[1]*0.1f * color_weight, currentCL[2]*0.2f * color_weight, 0.4f) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_CURRENT_BIT) ;
					glColor4f(currentCL[0] * color_weight, currentCL[1] * color_weight, currentCL[2] * color_weight, 0.4f) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}
}


/*
// ���ѡ�ʸ���׾�Σ����Ĥ��������-real ���ץ�����
inline void DrawMirrorHedrons2(GLfloat radius, GLfloat h_rotate, int blend = FALSE)
{
	int	     i ;
	GLfloat rad ;

	if (!blend) glDisable(GL_BLEND) ;
	glShadeModel(GL_FLAT) ;
//	glColorMaterial(GL_AMBIENT_AND_DIFFUSE) ;
//	glCallList(MATERIAL_GOLD_DIFFUSE) ;
	glCallList(MATERIAL_GOLD_SPECULAR) ;

	for (i = 0 ; i < 12 ; i ++)
	{
		glPushMatrix() ;

			rad = 30.0f * i ;

			glRotatef(rad, 0.0f,0.0f,-1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			glRotatef(h_rotate, 0.0f,1.0f,0.0f) ;	// ��������ž

			if (i == 0)
			{
				glPushAttrib(GL_LIGHTING_BIT) ;
//					glCallList(MATERIAL_RED_DIFFUSE) ;
					glCallList(MATERIAL_RED_SPECULAR) ;
					glCallList(HEDRON_LIST2) ;
				glPopAttrib() ;
			}
			else if (i == 6)
			{
				glPushAttrib(GL_LIGHTING_BIT) ;
					glCallList(MATERIAL_IRON_SPECULAR) ;
					glCallList(HEDRON_LIST3) ;
				glPopAttrib() ;
			}
			else
				glCallList(HEDRON_LIST) ;

		glPopMatrix() ;
	}

//	if (!blend) glEnable(GL_BLEND) ;
}
*/

/*
void DrawGlass1(GLfloat radius)
{
	int	flag = TRUE, divide = 60 ;

	glTranslatef(0.0f, 0.0f, 2.4f) ;
	glDisable(GL_LIGHTING) ;

//	glDisable(GL_FOG) ;
	glFogfv(GL_FOG_COLOR, black) ;
	for (int i = 0 ; i < divide ; i ++)
	{
		GLfloat alpha, rad = -2.0f * PAI / divide * i ;
		alpha = Pow(sin(rad*1.3f), 32.0f + i) ;

		if (alpha <= 0.3f)
		{
			if (flag)
				continue ;

			rad = -2.0f * PAI / divide * i ;
//			glColor4f(currentGC[0],currentGC[1],currentGC[2], 0.0f) ;
			glColor3f(0.0f, 0.0f, 0.0f) ;
			glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
			glEnd() ;
			flag = TRUE ;
			continue ;
		}

		if (flag)
		{
			GLfloat pRad = -2.0f * PAI / divide * (i - 1) ;
			flag = FALSE ;
			glBegin(GL_TRIANGLE_FAN) ;
			glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ
//			glColor4f(currentGC[0],currentGC[1],currentGC[2], 0.0f) ;
			glColor3f(0.0f, 0.0f, 0.0f) ;
			glVertex3f(sin(pRad) * radius, cos(pRad) * radius, 0.0f) ;
		}

//		glColor4f(currentGC[0],currentGC[1],currentGC[2], alpha) ;
		glColor3f(currentGC[0]*alpha,currentGC[1]*alpha,currentGC[2]*alpha) ;

		glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
	}
	if (!flag) glEnd() ;

//	glEnable(GL_FOG) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	glEnable(GL_LIGHTING) ;
}
*/


void SetGlass1Material(GLfloat color)
{
	if (vertexShadingFlag)
		SetMaterial(GL_FRONT, color, color, color, 1.0f, 0.3f, 0.5f * color * spec * specular, 32.0f) ;
//		SetMaterial(GL_FRONT, color, color, color, 1.0f, 0.3f, 0.5f * color * spec * specular, 32.0f, FALSE, 0.0f, 0.0f) ;	// ambient 0
	else
		glColor3f(currentGC[0]* color * spec * masterBright,
				  currentGC[1]* color * spec * masterBright,
				  currentGC[2]* color * spec * masterBright) ;
}


// Glass1
int DrawGlass1(GLfloat radius, int drawFlag = TRUE)
{
	int nPolygons = 2 + 5 + 3 ;

	// �Ρ��ޥ�Ϻ٤ᡢ����ʳ��Ͼ��������
	float thickness ;
	if (mode_normal)
		thickness = -2.0f ;
	else
		thickness = -4.0f ;

	if (!drawFlag)
		return nPolygons ;

	static GLfloat a1[] = { 0.0f, 0.9f, 0.0f } ;
	static GLfloat a2[] = { 0.0f, 0.6f, 0.0f, 0.8f, 0.5f, 0.0f } ;
	static GLfloat a3[] = { 0.0f, 0.5f, 0.2f, 0.0f } ;
	static int divide = 60 ;

	glTranslatef(0.0f, 0.0f, 2.4f) ;

//	if (!vertexShadingFlag)
//		glDisable(GL_LIGHTING) ;

	glFogfv(GL_FOG_COLOR, black) ;

	int r = 10 ;
	glBegin(GL_TRIANGLE_FAN) ;

	// �ǽ�ΰ��������å�
	SetGlass1Material(a1[0]) ;
	if (vertexShadingFlag)
		glNormal3f(0.0f, 0.0f, 1.0f) ;

	glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ
	for (int j = 0 ; j <= 2 ; j ++)
	{
		int i = j + r ;
		GLfloat rad = thickness * PAI / divide * i ;
		if (j) SetGlass1Material(a1[j]) ;
		glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
	}
	glEnd() ;

	r = 30 ;
	glBegin(GL_TRIANGLE_FAN) ;
	glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ
	for (int j = 0 ; j <= 5 ; j ++)
	{
		int i = j + r ;
		GLfloat rad = thickness * PAI / divide * i ;
		if (j) SetGlass1Material(a2[j]) ;
		glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
	}
	glEnd() ;

	r = 50 ;
	glBegin(GL_TRIANGLE_FAN) ;
	glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ
	for (int j = 0 ; j <= 3 ; j ++)
	{
		int i = j + r ;
		GLfloat rad = thickness * PAI / divide * i ;
		if (j) SetGlass1Material(a3[j]) ;
		glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
	}
	glEnd() ;

	glFogfv(GL_FOG_COLOR, currentBG) ;

//	if (!vertexShadingFlag)
//		glEnable(GL_LIGHTING) ;

	return nPolygons ;
}


// ɽ�̥��饹����ʻ��Ѥ����ݥꥴ������֤���
int DrawGlass(GLint divide, GLfloat radius, GLint light_switch = TRUE, float *s = NULL, float *t = NULL)
{
	if (cullObj)
		return 0 ;

	int	i, n_pols = 0 ;

//	glDisable(GL_FOG) ;
	glFogfv(GL_FOG_COLOR, black) ;

	switch (clock_glass)
	{
	case 0:
		break ;

	case 1:
		n_pols = DrawGlass1(radius, vertexShadingFlag) ;	// �������ǥ��󥰤ʤ��ξ���-s 1 �Τߡˤϥ��ߡ�
		break ;

	case 2:
		glBegin(GL_TRIANGLE_FAN) ;

			if (light_switch)
			{
				// glCallList(MATERIAL_GLASS) ;
//				SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.5f * spec * specular, 128.0f) ;

//				if (mode_solid == 2)
//					SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.0f) ;
//				else
					SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.5f * spec, 128.0f) ;
			}

			if (flag_glNormal) glNormal3f(0.0f, 0.0f, 1.0f) ;	// �濴��ˡ���٥��ȥ�
			if (s && t)
				TexCoord2f(0.0f, 0.0f) ;
//				TexCoord2f(s[0] * 0.0f + s[1] * 0.0f + s[2] * 0.0f, t[0] * 0.0f + t[1] * 0.0f + t[2] * 0.0f) ;
			glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ

			if (light_switch)	// glCallList(MATERIAL_GLASS2) ;
			{
//				SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 1.0f * spec * specular, 128.0f) ;

//				if (mode_solid == 2)
//					SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.0f) ;
//				else
					SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.9f * spec, 128.0f) ;
			}

			for (i = 0 ; i <= divide ; i ++)
			{
				GLfloat rad = -2.0f * PAI / divide * i ;
				if (i == divide) rad = 0.0f ;
				GLfloat x = sin(rad), y = cos(rad) ;
				Vector3d norm(x * 0.15f * norm_ratio, y * 0.15f * norm_ratio, 1.0f) ;

				if (flag_glNormal) glNormal3fv(norm.Unit().v) ;	// ˡ���٥��ȥ�Ͼ�����������
				if (s && t)
					TexCoord2f(s[0] * x * radius + s[1] * y * radius + s[2] * 0.0f, t[0] * x * radius + t[1] * y * radius + t[2] * 0.0f) ;
				glVertex3f(x * radius, y * radius, 0.0f) ;
			}

		glEnd() ;
//		if (light_switch) glEnable(GL_LIGHTING) ;

		n_pols = divide ;
		break ;

	case 3:
		{
			if (light_switch)
			{
//				glCallList(MATERIAL_GLASS4) ;
//				if (mode_solid == 2)
//					SetMaterial(GL_FRONT_AND_BACK, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.0f) ;
//				else
					SetMaterial(GL_FRONT_AND_BACK, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.75f * spec, 128.0f) ;

				// �����׻�������
				if (!lightTwoSide)
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;
			}

			RollingObject glass3(divide * (6 - 1)) ;
			glass3.MakeRollingPolygons(divide, 6, glass_v) ;
			glass3.AverageNormals(TRUE, TRUE) ;
			glass3.TopNormal() ;

			glPushAttrib(GL_CULL_FACE) ;
				glDisable(GL_CULL_FACE) ;
				glass3.DrawObject(flag_glNormal, s, t) ;
			glPopAttrib() ;

			if (light_switch)
			{
//				glEnable(GL_LIGHTING) ;
				// �����׻������������᤹
				if (!lightTwoSide)
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
			}

			n_pols = divide * 9 ;
			break ;
		}
	}

//	glEnable(GL_FOG) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	return n_pols ;
}


// �ÿˤ����⡼�����֥顼�ѤΥ������ȥ��󥰥뤪��ӥ��ƥå��ͤ�׻�
void CalcNeedleMotionBlurAngles(ClockState& pre, ClockState& now)
{
	now.start_angle = pre.tm.second ;

	// ����γ��٤��������������ϡ��������̤äƤ��뤿�ᡢ�������� - 360
//	if (now.tm.second >= start) now.start_angle = pre.tm.second ;
//	else			    now.start_angle = pre.tm.second - 360.0f ;

	now.start_angle = pre.tm.second ;

	// ����å����ˤ�äƥ������ȳ��٤�Ĵ��
	now.start_angle += (now.tm.second - now.start_angle) * (1.0f - s_shutter) ;
	// ���ƥå��� = (���߳��� - �������ȳ���) / (������󥰲�� - 1 + 1)
	// shutter == 1.0f �Ǥ���ʬ���餹����
	now.step_angle = (now.tm.second - now.start_angle) / (s_motion) ;
	now.start_angle += now.step_angle ;
}

// ����꣱������
// -solid 1 �Τߡ�����ˡ������ꤷ�ʤ�
void DrawGoldenObject(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f,
					  int depth = FALSE, int average = TRUE, int smooth = FALSE, int normalFlag = TRUE,
					  float *s = NULL, float *t = NULL)
{
	int      i ;
	Object   gold(4) ;
	GLfloat m[] = {   x, 0.0f, 0.0f, 0.0f,
					 0.0f,   y, 0.0f, 0.0f,
					 0.0f, 0.0f,   z, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f } ;

	for (i = 0 ; i < 4 ; i ++) gold.MakePolygon(i, g_o[i][0],g_o[i][1],g_o[i][2], g_v, m) ;

	if (smooth)
	{
		// �ݥꥴ�󣰤�ˡ��
		gold.AverageNormal(0, 0,  0,1,2,3) ;
		gold.AverageNormal(0, 1,  0,2) ;
		gold.AverageNormal(0, 2,  0,1) ;

		// �ݥꥴ�󣱤�ˡ��
		gold.AverageNormal(1, 0,  0,1,2,3) ;
		gold.AverageNormal(1, 1,  0,1) ;
		gold.AverageNormal(1, 2,  1,3) ;

		// �ݥꥴ�󣲤�ˡ��
		gold.AverageNormal(2, 0,  0,1,2,3) ;
		gold.AverageNormal(2, 1,  2,3) ;
		gold.AverageNormal(2, 2,  0,2) ;

		// �ݥꥴ�󣳤�ˡ��
		gold.AverageNormal(3, 0,  0,1,2,3) ;
		gold.AverageNormal(3, 1,  1,3) ;
		gold.AverageNormal(3, 2,  2,3) ;

		gold.DrawTriangles(depth, average, FALSE, normalFlag, s, t) ;
	}
	else
		gold.DrawTriangles(depth, average, TRUE, normalFlag, s, t) ;	// �ե�åȻ���
}

// ���������
void DrawGoldenObjects(GLfloat radius, GLfloat div = 6, float *s = NULL, float *t = NULL)
{
	if (gold_depth < 0)
		return ;

	int i ;
	for (i = 0 ; i < div ; i ++)
	{
		glPushMatrix() ;
		{
			glRotatef(-360.0f / div * i, 0.0f,0.0f,1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
//			glCallList(GOLDEN_OBJECT_LIST) ;

/*
			int smoothFlag ;
			if (mode_solid == 1 || (mode_normal && !light))
				smoothFlag = TRUE ;
			else if (g_smooth < 0)	// -gsmooth -1�ʥǥե���ȡˤξ��ϼ�ưĴ��
				smoothFlag = texture ;
			else
				smoothFlag = g_smooth ;
*/
			DrawGoldenObject(1.1f,1.1f,1.0f, gold_depth, TRUE /*average*/, g_smooth, flag_glNormal, s, t) ;
		}
		glPopMatrix() ;
	}
}


// ���顼����Ĵ�������סʥ������ǥ��󥰤ʤ���-s 1 �ѡ�
// FilterTest ����
void DrawSolid1ModulateClockWithFilterTest(ClockState& st)
{
//	glEnable(GL_DEPTH_TEST) ;
	glDepthMask(GL_TRUE) ;

	glColor3fv(white) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;
	
		glEnable(GL_CULL_FACE) ;
	}

//	glShadeModel(GL_FLAT) ;		// �ե�åȤˤ����;�פ��٤��ʤ��

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
//	if (!noCullObj && zAxis > CULL_SHELL_Z_AXIS)
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// -solid 1 && filterTest �ξ��϶����ϥ�����˴ޤޤ�Ƥ��뤿������
//	if (gold_depth >= 0)
//		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
//	if (!noCullObj && zAxis < CULL_BOARD_Z_AXIS)
	if (cull_status & CULL_STATUS_FRONT)
	{
//		glShadeModel(GL_SMOOTH) ;
		return ;
	}

	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;
	{

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawModulateHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor3f(currentCL[0]*COLOR_HOUR_SOLID_R, currentCL[1]*COLOR_HOUR_SOLID_G, currentCL[2]*COLOR_HOUR_SOLID_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor3f(currentCL[0]*COLOR_MINUTE_R, currentCL[1]*COLOR_MINUTE_G, currentCL[2]*COLOR_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
			{
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
				glColor4f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B, 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			}
			glPopAttrib() ;

			// �Ǹ�Σ�����Ǥǥ������
			glColor3f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glColor3f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�

			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}


		// ɽ�̥��饹����
		if (clock_glass)
		{
			glBlendFunc(GL_ONE, GL_ONE) ;
//			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			glFogfv(GL_FOG_COLOR, black) ;

/*
			if (clock_glass == 1)
			{
				// ��ž
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;

				if ((timeData.n_second == 42 && timeData.p_second != timeData.n_second) || start)
				{
					glDeleteLists(GLASS_LIST, 1) ;
					glNewList(GLASS_LIST, GL_COMPILE_AND_EXECUTE) ;
						DrawGlass1(14.4f) ;		// ��ʬ��˥ꥹ�Ⱥ����
					glEndList() ;
				}
				else
					glCallList(GLASS_LIST) ;
			}
			else
*/
			{
				glColor3f(currentCL[0] * 0.4f, currentCL[1] * 0.4f, currentCL[2] * 0.4f) ;
				glCallList(ENV_GLASS_LIST) ;
			}

			glFogfv(GL_FOG_COLOR, currentBG) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}
		else
		{
			glDepthMask(GL_TRUE) ;
		}
	}

	glPopMatrix() ;
}


// ���顼����Ĵ�������סʥ������ǥ��󥰤ʤ���-s 1 �ѡ�
void DrawSolid1ModulateClock(ClockState& st)
{
//	glDisable(GL_TEXTURE_2D) ;


//	glEnable(GL_DEPTH_TEST) ;
	glDepthMask(GL_TRUE) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

//	glShadeModel(GL_FLAT) ;		// �ե�åȤˤ����;�פ��٤��ʤ��

	glColor3f(currentCL[0], currentCL[1], currentCL[2]*0.8f) ;
//	glColor3fv(currentCL) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
//	if (!noCullObj && zAxis > CULL_SHELL_Z_AXIS)
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	if (!filterTest)
		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;

	if (gold_depth >= 0)
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
//	if (!noCullObj && zAxis < CULL_BOARD_Z_AXIS)
	if (cull_status & CULL_STATUS_FRONT)
	{
//		glShadeModel(GL_SMOOTH) ;
		return ;
	}


	// MATERIAL_RED_GRAY �����å�
	glColor3f(currentCL[0], currentCL[1]*0.7f, currentCL[2]*0.7f) ;
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;
	{
		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawModulateHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor3f(currentCL[0]*COLOR_HOUR_SOLID_R, currentCL[1]*COLOR_HOUR_SOLID_G, currentCL[2]*COLOR_HOUR_SOLID_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor3f(currentCL[0]*COLOR_MINUTE_R, currentCL[1]*COLOR_MINUTE_G, currentCL[2]*COLOR_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
			{
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
				glColor4f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B, 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			}
			glPopAttrib() ;

			// �Ǹ�Σ�����Ǥǥ������
			glColor3f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glColor3f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�

			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glBlendFunc(GL_ONE, GL_ONE) ;
//			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			glFogfv(GL_FOG_COLOR, black) ;

/*
			if (clock_glass == 1)
			{
				// ��ž
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;

				if ((timeData.n_second == 42 && timeData.p_second != timeData.n_second) || start)
				{
					glDeleteLists(GLASS_LIST, 1) ;
					glNewList(GLASS_LIST, GL_COMPILE_AND_EXECUTE) ;
						DrawGlass1(14.4f) ;		// ��ʬ��˥ꥹ�Ⱥ����
					glEndList() ;
				}
				else
					glCallList(GLASS_LIST) ;
			}
			else
*/
			{
				glColor3f(currentCL[0] * 0.4f, currentCL[1] * 0.4f, currentCL[2] * 0.4f) ;
				glCallList(ENV_GLASS_LIST) ;
			}

			glFogfv(GL_FOG_COLOR, currentBG) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}
		else
		{
			glDepthMask(GL_TRUE) ;
		}
	}

	glPopMatrix() ;
}


// ���顼����Ĵ�������סʥ������ǥ��󥰤ʤ���-s 1 �ѡ�
void DrawSolid1ModulateClockWithFilterTestAndOverSampling(ClockState& st, GLfloat alpha_weight = 1.0f, int n = 0)
{
	glColor4f(1.0f, 1.0f, 1.0f, alpha_weight) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], alpha_weight) ;
			glCallList(ENV_ROOF_OVER_LIST[n]) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}

	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;
	{
		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedronsWithOverSampling(radius_ratio * HDRN_RADIUS, st.h_rotate, alpha_weight) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor4f(currentCL[0]*COLOR_HOUR_SOLID_R, currentCL[1]*COLOR_HOUR_SOLID_G, currentCL[2]*COLOR_HOUR_SOLID_B, alpha_weight) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_MINUTE_R, currentCL[1]*COLOR_MINUTE_G, currentCL[2]*COLOR_MINUTE_B, alpha_weight) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B, alpha_weight) ;	// MATERIAL_MAT_GOLD �����å�
		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glBlendFunc(GL_ONE, GL_ONE) ;
			glEnable(GL_BLEND) ;

			glFogfv(GL_FOG_COLOR, black) ;

			glColor3f(currentCL[0] * 0.4f * alpha_weight, currentCL[1] * 0.4f * alpha_weight, currentCL[2] * 0.4f * alpha_weight) ;
			glCallList(ENV_GLASS_LIST) ;

			glFogfv(GL_FOG_COLOR, currentBG) ;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		}
	}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// ���顼����Ĵ�������סʥ������ǥ��󥰤ʤ���-s 1 �ѡ�
void DrawSolid1ModulateClockWithOverSampling(ClockState& st, GLfloat alpha_weight = 1.0f, int n = 0)
{
	if (filterTest)	// || orgImageFlag)
	{
//		DrawSolid1ModulateClockWithFilterTest(st) ;
		DrawSolid1ModulateClockWithFilterTestAndOverSampling(st, alpha_weight, n) ;
		return ;
	}

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], alpha_weight) ;
			glCallList(ENV_ROOF_OVER_LIST[n]) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	glColor4f(currentCL[0], currentCL[1], currentCL[2]*0.8f, alpha_weight) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, alpha_weight) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	// MATERIAL_RED_GRAY �����å�
	glColor4f(currentCL[0], currentCL[1]*0.7f, currentCL[2]*0.7f, alpha_weight) ;
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;
	{
		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedronsWithOverSampling(radius_ratio * HDRN_RADIUS, st.h_rotate, alpha_weight) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor4f(currentCL[0]*COLOR_HOUR_SOLID_R, currentCL[1]*COLOR_HOUR_SOLID_G, currentCL[2]*COLOR_HOUR_SOLID_B, alpha_weight) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_MINUTE_R, currentCL[1]*COLOR_MINUTE_G, currentCL[2]*COLOR_MINUTE_B, alpha_weight) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_SECOND_R, currentCL[1]*COLOR_SECOND_G, currentCL[2]*COLOR_SECOND_B, alpha_weight) ;	// MATERIAL_MAT_GOLD �����å�
		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glBlendFunc(GL_ONE, GL_ONE) ;
			glEnable(GL_BLEND) ;

			glFogfv(GL_FOG_COLOR, black) ;

			glColor3f(currentCL[0] * 0.4f * alpha_weight, currentCL[1] * 0.4f * alpha_weight, currentCL[2] * 0.4f * alpha_weight) ;
			glCallList(ENV_GLASS_LIST) ;

			glFogfv(GL_FOG_COLOR, currentBG) ;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		}
	}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// -e 1, 2 �Υ������ǥ��󥰤ʤ�������ʬ
// ��ž���֥������ȡ��ˡ����饹
void DrawEnvModulate_texture_object(ClockState& st)
{
	glPushMatrix() ;
	{
		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;

		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
			{
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
				glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			}
			glPopAttrib() ;

			glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
//			glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�

			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
		{
//			switch (env)
//			{
//			case 1:
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;

//				glCallList(ENV_GLASS_TEXTURE_LIST) ;
				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;

				glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

				glDisable(GL_BLEND) ;
				glDepthMask(GL_TRUE) ;
/*
				break ;

			case 2:	// env == 2 �λ�
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;
				break ;
			}
*/
		}
		else
			glDepthMask(GL_TRUE) ;
	}
	glPopMatrix() ;
}


// -e 2 ��
// �������ǥ��󥰡ܥ��ڥ���顼��Texture Object ����
void DrawEnv2Clock_texture_object(ClockState& st)
{
	if (!detail)
	{
		// �ƥ�������ϥ��饤���ѥ֥��ɥ⡼��
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;

		glEnable(GL_TEXTURE_2D) ;	// Mesa �Ǥ�ʸ������θ�ƥ������㤬���դΤޤ�
	}
	else	// detail == 5
	{
		// �ǥ��ƥ�����ƥ��������ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE0) ;
		glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;	// �Ķ��ޥå׻��ˤϤ�����ɬ��
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;

		glEnable(GL_TEXTURE_2D) ;	// Mesa �Ǥ�ʸ������θ�ƥ������㤬���դΤޤ�

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι���μ¤˥��ꥢ��
		{
			glMatrixMode(GL_TEXTURE) ;
			glLoadIdentity() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// �ƥ�������ϥ��饤���ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE1) ;
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;	// detail ���ˤ� GL_TEXTURE1
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;
		glEnable(GL_TEXTURE_2D) ;
	}

	// �饤�ƥ��� ON
	glEnable(GL_LIGHTING) ;

	glDepthMask(GL_TRUE) ;

	// MATERIAL_MAT_IRON �����å�
//	glColor3fv(currentCL) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
	// ���Υꥹ�������ǴĶ��ƥ�������򥹥��å����뤿�ᡢ
	// detail ���� glActiveTexture(GL_TEXTURE1) �ˤʤäƤ���ɬ�פ��ꡪ
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;

//	glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
//	glCallList(MATERIAL_GOLD) ;
//	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		if (detail)
		{
			// TEXTURE1 OFF
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;

			// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
			glActiveTexture(GL_TEXTURE0) ;
/*
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;

			if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
			{
				glMatrixMode(GL_TEXTURE) ;
//				glLoadIdentity() ;
				glScalef(0.5f, 0.5f, 0.5f) ;
				glTranslatef(1.0f, 1.0f, 1.0f) ;
				glMatrixMode(GL_MODELVIEW) ;
			}

			// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
*/
		}

		return ;
	}

	// MATERIAL_RED_GRAY �����å�
	glBindTexture(GL_TEXTURE_2D, PINK_TEXTURE_NAME) ;	// detail ���ˤ� GL_TEXTURE1
	glCallList(BOARD_LIST) ;

	// �ʹߥ饤�ƥ��󥰤ʤ�
	// -e 1 �ȶ���

	// �饤�ƥ��� OFF
	glDisable(GL_LIGHTING) ;

	if (detail)
	{
		// TEXTURE1 OFF
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
		glActiveTexture(GL_TEXTURE0) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
		{
			glMatrixMode(GL_TEXTURE) ;
//			glLoadIdentity() ;
//			glScalef(0.5f, 0.5f, 0.5f) ;
//			glTranslatef(1.0f, 1.0f, 1.0f) ;
			ScaleTranslateMatrixForFake() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
	}
	else
	{
		// �ƥ�������֥��ɥ⡼�ɤ��ǥ�졼�Ȥ��᤹
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	}

	DrawEnvModulate_texture_object(st) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// ���顼����Ĵ������������ʥ������ǥ��󥰤ʤ��� Texture Object ��
void DrawModulateClock_texture_object(ClockState& st)
{
//	if (mesa)	// �ʤ����ƥ������㤬ȴ����
		glEnable(GL_TEXTURE_2D) ;


	if (detail)
	{
		// �ǥ��ƥ�����ƥ��������ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE0) ;
		glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι���μ¤˥��ꥢ��
		{
			glMatrixMode(GL_TEXTURE) ;
			glLoadIdentity() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// �ƥ�������ϥ��饤���ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE1) ;
		glEnable(GL_TEXTURE_2D) ;
	}

	if (filterTest)
	{
		glColor3f(1.0f, 1.0f, 1.0f) ;

		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

	else
	{
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

//	glEnable(GL_DEPTH_TEST) ;
	glDepthMask(GL_TRUE) ;


	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	if (!filterTest)
	{
		glColor3fv(currentCL) ;
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
	if (!filterTest)
	{
//		glColor3fv(currentCL) ;
		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
	}

	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		if (detail)
		{
			// TEXTURE1 OFF
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;

			// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
			glActiveTexture(GL_TEXTURE0) ;

/*
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;

			if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
			{
				glMatrixMode(GL_TEXTURE) ;
//				glLoadIdentity() ;
				glScalef(0.5f, 0.5f, 0.5f) ;
				glTranslatef(1.0f, 1.0f, 1.0f) ;
				glMatrixMode(GL_MODELVIEW) ;
			}

			// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
*/
		}

		return ;
	}

	// MATERIAL_RED_GRAY �����å�
	if (!filterTest)
	{
//		glColor3f(currentCL[0], currentCL[1]*0.6f, currentCL[2]*0.6f) ;
		GLfloat alpha = ReflectAlphaByCos(zAxis, 0.6f, 3.0f) ;
		glColor3f(currentCL[0], currentCL[1] * alpha, currentCL[2] * alpha) ;
		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glBindTexture(GL_TEXTURE_2D, PINK_TEXTURE_NAME) ;
	}

	glCallList(ENV_BOARD_LIST) ;

	if (detail)
	{
		// TEXTURE1 OFF
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
		glActiveTexture(GL_TEXTURE0) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
		{
			glMatrixMode(GL_TEXTURE) ;
//			glLoadIdentity() ;
//			glScalef(0.5f, 0.5f, 0.5f) ;
//			glTranslatef(1.0f, 1.0f, 1.0f) ;
			ScaleTranslateMatrixForFake() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
	}

	DrawEnvModulate_texture_object(st) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// ���顼����Ĵ������������ʥ������ǥ��󥰤ʤ���
// Texture Object �ʤ�
void DrawModulateClock(ClockState& st)
{
//	if (mesa)	// �ʤ����ƥ������㤬ȴ����
		glEnable(GL_TEXTURE_2D) ;

// 99-09-23
//	glDisable(GL_TEXTURE_GEN_S) ;
//	glDisable(GL_TEXTURE_GEN_T) ;

	if (filterTest)
	{
		glColor3f(1.0f, 1.0f, 1.0f) ;
	}


//	glEnable(GL_DEPTH_TEST) ;
	glDepthMask(GL_TRUE) ;


	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	if (!filterTest)
		glColor3f(currentCL[0], currentCL[1], currentCL[2]*0.8f) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�äƳ̥��֥������Ȥ��ڤ��ؤ�
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	if (!filterTest)
		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;

	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}

	// MATERIAL_RED_GRAY �����å�
	if (!filterTest)
	{
//		glColor3f(currentCL[0], currentCL[1]*0.6f, currentCL[2]*0.6f) ;
		GLfloat alpha = ReflectAlphaByCos(zAxis, 0.6f, 3.0f) ;
		glColor3f(currentCL[0], currentCL[1] * alpha, currentCL[2] * alpha) ;
	}
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;

		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�

		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
				glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B, 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//			glBlendFunc(GL_ONE, GL_ONE) ;
//#endif

			// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
//			glPushMatrix() ;
//				glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//				glCallList(ENV_SECOND_LIST) ;
//			glPopMatrix() ;

			// �Ǹ�Σ�����Ǥǥ������
			glColor3f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glColor3f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�

			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
		{
			switch (env)
			{
/*
			case 0:
				glBlendFunc(GL_ONE, GL_ONE) ;
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;

				glColor3f(currentCL[0] * 0.4f, currentCL[1] * 0.4f, currentCL[2] * 0.4f) ;

				glCallList(ENV_GLASS_LIST) ;

				glDisable(GL_BLEND) ;
				glDepthMask(GL_TRUE) ;

				break ;
*/
			case 1:
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
				glEnable(GL_BLEND) ;
				glDepthMask(GL_FALSE) ;

				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
				{
					glCallList(ENV_GLASS_TEXTURE_LIST) ;
					glMatrixMode(GL_MODELVIEW) ;

					glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
					glCallList(ENV_GLASS_LIST) ;

					glMatrixMode(GL_TEXTURE) ;
				}
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;

				glDisable(GL_BLEND) ;
				glDepthMask(GL_TRUE) ;

				break ;

			default:	// env == 2 �λ�
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;
				break ;
			}

		}
		else
			glDepthMask(GL_TRUE) ;

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// �����Х���ץ���դ����顼����Ĵ������������ʥ������ǥ��󥰤ʤ���
// �ƥ������㡦���֥�����������
void DrawModulateClockWithOverSampling_texture_object(ClockState& st, GLfloat alpha_weight = 1.0f, int n = 0)
{

	if (detail)
	{
		// �ǥ��ƥ�����ƥ��������ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE0) ;
		glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι���μ¤˥��ꥢ��
		{
			glMatrixMode(GL_TEXTURE) ;
			glLoadIdentity() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// �ƥ�������ϥ��饤���ѥ֥��ɥ⡼��
		glActiveTexture(GL_TEXTURE1) ;
		glEnable(GL_TEXTURE_2D) ;
	}

	if (filterTest)
	{
		glColor4f(1.0f, 1.0f, 1.0f, alpha_weight) ;

		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

	else
	{
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
//			if (!filterTest) glColor4f(currentCL[0], currentCL[1], currentCL[2]/* *0.8f*/, alpha_weight) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_OVER_LIST[n]) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	if (!filterTest)
	{
		glColor4f(currentCL[0], currentCL[1], currentCL[2], alpha_weight) ;
		glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
	if (!filterTest)
	{
//		glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, alpha_weight) ;
		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
	}

	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		if (detail)	// -e 1 -D -{m|a|f}
		{
			// TEXTURE1 OFF
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;

			// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
			glActiveTexture(GL_TEXTURE0) ;
/*
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;

			if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
			{
				glMatrixMode(GL_TEXTURE) ;
//				glLoadIdentity() ;
				glScalef(0.5f, 0.5f, 0.5f) ;
				glTranslatef(1.0f, 1.0f, 1.0f) ;
				glMatrixMode(GL_MODELVIEW) ;
			}

			// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
*/
		}

		return ;
	}


	// MATERIAL_RED_GRAY �����å�
	if (!filterTest)
	{
//		glColor4f(currentCL[0], currentCL[1]*0.6f, currentCL[2]*0.6f, alpha_weight) ;
		GLfloat alpha = ReflectAlphaByCos(zAxis, 0.6f, 3.0f) ;
		glColor4f(currentCL[0], currentCL[1] * alpha, currentCL[2] * alpha, alpha_weight) ;
		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glBindTexture(GL_TEXTURE_2D, PINK_TEXTURE_NAME) ;
	}

	glCallList(ENV_BOARD_LIST) ;

	if (detail)	// -e 1 -D -{m|a|f}
	{
		// TEXTURE1 OFF
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ǥ��ƥ�����ƥ��������Ѥ���Ķ��ޥå��������
		glActiveTexture(GL_TEXTURE0) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;

		if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE0 �ι��󥻥åȡ�
		{
			glMatrixMode(GL_TEXTURE) ;
//			glLoadIdentity() ;
//			glScalef(0.5f, 0.5f, 0.5f) ;
//			glTranslatef(1.0f, 1.0f, 1.0f) ;
			ScaleTranslateMatrixForFake() ;
			glMatrixMode(GL_MODELVIEW) ;
		}

		// GL_TEXTURE0 �Υǥ��ƥ�����ƥ�������Ͻ�ᤫ�� GL_MODULATE
	}


	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate, 1.0f, alpha_weight) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glEnable(GL_BLEND) ;
			glBlendFunc(GL_ONE, GL_ONE) ;

			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
			glColor3f(currentCL[0]*ENV_GLASS_BRIGHT * alpha_weight, currentCL[1]*ENV_GLASS_BRIGHT * alpha_weight, currentCL[2]*ENV_GLASS_BRIGHT * alpha_weight) ;
			glCallList(ENV_GLASS_LIST) ;

			glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// �����Х���ץ���դ����顼����Ĵ������������ʥ������ǥ��󥰤ʤ���
void DrawModulateClockWithOverSampling(ClockState& st, GLfloat alpha_weight = 1.0f, int n = 0)
{
	if (filterTest)
	{
//		glColor3f(1.0f, 1.0f, 1.0f) ;
		glColor4f(1.0f, 1.0f, 1.0f, alpha_weight) ;
	}

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
//			glColor4f(currentCL[0], currentCL[1], currentCL[2]*0.8f, alpha_weight) ;
			glCallList(ENV_ROOF_OVER_LIST[n]) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	if (!filterTest)
		glColor4f(currentCL[0], currentCL[1], currentCL[2]*0.8f, alpha_weight) ;


	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
	if (!filterTest)
		glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, alpha_weight) ;

	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	// MATERIAL_RED_GRAY �����å�
	if (!filterTest)
	{
//		glColor4f(currentCL[0], currentCL[1]*0.6f, currentCL[2]*0.6f, alpha_weight) ;
		GLfloat alpha = ReflectAlphaByCos(zAxis, 0.6f, 3.0f) ;
		glColor4f(currentCL[0], currentCL[1] * alpha, currentCL[2] * alpha, alpha_weight) ;
	}
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedronsWithOverSampling(radius_ratio * HDRN_RADIUS, st.h_rotate, alpha_weight) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor4f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B, alpha_weight) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B, alpha_weight) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B, alpha_weight) ;	// MATERIAL_MAT_GOLD �����å�
		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glEnable(GL_BLEND) ;
			glBlendFunc(GL_ONE, GL_ONE) ;
			switch (env)
			{
			case 1:
				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
					glCallList(ENV_GLASS_TEXTURE_LIST) ;

					glMatrixMode(GL_MODELVIEW) ;
					glColor3f(currentCL[0]*ENV_GLASS_BRIGHT * alpha_weight, currentCL[1]*ENV_GLASS_BRIGHT * alpha_weight, currentCL[2]*ENV_GLASS_BRIGHT * alpha_weight) ;
					glCallList(ENV_GLASS_LIST) ;

					glMatrixMode(GL_TEXTURE) ;
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;

				break ;

			case 2:	// env == 2 �λ�
				glColor3f(currentCL[0]*0.5f * alpha_weight, currentCL[1]*0.5f * alpha_weight, currentCL[2]*0.5f * alpha_weight) ;
				glCallList(ENV_GLASS_LIST) ;
				break ;
			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// -e 3 �ΰ���ܤ�����
void DrawMirrorClock2(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		// MATERIAL_GOLD �����å�
//		glColor3f(currentCL[0]*0.5f, currentCL[1]*0.45f, currentCL[2]*0.03f) ;
		glColor3f(currentCL[0]*0.3f*1.0f, currentCL[1]*0.3f*0.75f, currentCL[2]*0.3*0.05f) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	glColor3f(currentCL[0]*0.5f, currentCL[1]*0.5f, currentCL[2]*0.4f) ;

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
//	glColor3f(currentCL[0], currentCL[1]*0.9f, currentCL[2]*0.05f) ;
	glColor3f(currentCL[0]*0.7f*1.0f, currentCL[1]*0.7f*0.75f, currentCL[2]*0.7f*0.05f) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;



	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	// MATERIAL_RED_GRAY �����å�
	glColor3f(currentCL[0]*0.8f, currentCL[1]*0.5f, currentCL[2]*0.5f) ;
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glColor3f(currentCL[0]*0.5f, currentCL[1]*0.1f, currentCL[2]) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glColor3f(currentCL[0]*0.1f, currentCL[1]*0.6f,  currentCL[2]*0.2f) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ; //, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glEnable(GL_BLEND) ;
//				glDisable(GL_DEPTH_TEST) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
				glColor4f(currentCL[0]*1.0f, currentCL[1]*0.75f, currentCL[2]*0.05f, 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
				{
//					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;

//			// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
//			glPushMatrix() ;
//				glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//				glCallList(ENV_SECOND_LIST) ;
//			glPopMatrix() ;

			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glColor3f(currentCL[0]*1.0f, currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}

/*
		// ɽ�̥��饹����
		if (clock_glass)
		{
			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			glColor3f(currentCL[0] * 0.4f, currentCL[1] * 0.4f, currentCL[2] * 0.4f) ;
			glCallList(ENV_GLASS_LIST) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}
*/

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// �徽��������ʥ��ơ������ǡ�
void DrawCrystalClock(ClockState& st, float color_weight = 1.0f)
{
//	glBlendFunc(GL_ONE, GL_ONE) ;
	glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;

	glDisable(GL_CULL_FACE) ;

	if (!lid)
	{
//		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
//			glColor4f(currentCL[0]*0.7f*0.75f, currentCL[1]*0.7f*0.75f, currentCL[2]*0.7f*0.75f, crystal_a) ;
			glColor3f(currentCL[0]*realitySpecularBright * color_weight, currentCL[1]*realitySpecularBright * color_weight, currentCL[2]*realitySpecularBright * color_weight) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		}
		glPopMatrix() ;
	}

//	glEnable(GL_CULL_FACE) ;

	// MATERIAL_MAT_IRON �����å�
//	glColor4f(currentCL[0]*0.7f, currentCL[1]*0.7f, currentCL[2]*0.7f * 0.8f, crystal_a) ;
	glColor3f(currentCL[0]*realitySpecularBright * color_weight, currentCL[1]*realitySpecularBright * color_weight, currentCL[2]*realitySpecularBright * 0.8f * color_weight) ;
	glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
//	glColor4f(currentCL[0]*0.7f, currentCL[1]*0.7f*0.6f, currentCL[2]*0.7f*0.15f, crystal_a) ;
	glColor3f(currentCL[0]*realitySpecularBright * color_weight, currentCL[1]*realitySpecularBright * 0.75f * color_weight, currentCL[2]*realitySpecularBright * 0.1f * color_weight) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

	// MATERIAL_RED_GRAY �����å�
//	glColor4f(currentCL[0]*0.7f*0.8f, currentCL[1]*0.7f*0.6f, currentCL[2]*0.7f*0.6f, crystal_a) ;
	glColor3f(currentCL[0]*realitySpecularBright * color_weight, currentCL[1]*realitySpecularBright * 0.7f * color_weight, currentCL[2]*realitySpecularBright * 0.7f * color_weight) ;
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;
	{
		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawCrystalHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate, color_weight) ;
//		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
//		glColor4f(currentCL[0]*0.7f*0.75f, currentCL[1]*0.7f*0.2f, currentCL[2]*0.7f, crystal_a) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glColor3f(currentCL[0] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_HOUR_R * color_weight,
				  currentCL[1] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_HOUR_G * color_weight,
				  currentCL[2] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_HOUR_B * color_weight) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		}
		glPopMatrix() ;

//		glColor4f(currentCL[0]*0.7f*0.2f, currentCL[1]*0.7f, currentCL[2]*0.7f*0.3f, crystal_a) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glColor3f(currentCL[0] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_MINUTE_R * color_weight,
				  currentCL[1] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_MINUTE_G * color_weight,
				  currentCL[2] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_MINUTE_B * color_weight) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		}
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;

			for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
			{
				// ����ե������θ���� MATERIAL_MAT_GOLD �����å�
//				glColor4f(currentCL[0]/s_motion, currentCL[1]*0.7f*0.75f/s_motion, currentCL[2]*0.7f*0.05f/s_motion, crystal_a) ;
				glColor3f(currentCL[0] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_R / s_motion,
						  currentCL[1] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_G / s_motion,
						  currentCL[2] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_B / s_motion) ;
				glPushMatrix() ;
				{
					glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_SECOND_LIST) ;
				}
				glPopMatrix() ;
			}
		}
		else
		{
//			glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, crystal_a) ;	// MATERIAL_MAT_GOLD �����å�
			glColor3f(currentCL[0] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_R * color_weight,
					  currentCL[1] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_G * color_weight,
					  currentCL[2] * CRYSTAL_NEEDLE_SPECULAR_ADD * COLOR_CHROME_SECOND_B * color_weight) ;
			glPushMatrix() ;
			{
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			}
			glPopMatrix() ;
		}
		// �֥��ɤλ������ѹ�
		if (crystal == 2) glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) ;

		// ɽ�̥��饹����
		if (clock_glass)
		{
/*
			glEnable(GL_LIGHTING) ;
			glCallList(GLASS_LIST) ;
			glDisable(GL_LIGHTING) ;
*/
			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;


			if (has_texture_object)
			{
				// �ƥ������㡦���֥���������
				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
//				glCallList(ENV_GLASS_TEXTURE_LIST) ;

				glColor3f(currentCL[0]*envGlassBright * color_weight, currentCL[1]*envGlassBright * color_weight, currentCL[2]*envGlassBright * color_weight) ;
				glCallList(ENV_GLASS_LIST) ;

				glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
			}
			else
			{
				// �ƥ������㡦���֥������Ȥʤ�
				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
				{
					glCallList(ENV_GLASS_TEXTURE_LIST) ;
					glMatrixMode(GL_MODELVIEW) ;

					glColor3f(currentCL[0]*envGlassBright * color_weight, currentCL[1]*envGlassBright * color_weight, currentCL[2]*envGlassBright * color_weight) ;
					glCallList(ENV_GLASS_LIST) ;

					glMatrixMode(GL_TEXTURE) ;
				}
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;
			}
		}
	}

	glPopMatrix() ;
}


// ����åɥƥ�������ξ夫�顢�ϥ��饤�Ȥ�Ťͤ�
void DrawSpecularClock(ClockState& st)
{
//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif

	glEnable(GL_BLEND) ;
	// DrawSignAndFPS() ��ǥ��åȤ����֥��ɥե��������ꥹ�ȥ�����ʤ���
	glBlendFunc(GL_ONE, GL_ONE) ;

	glDepthMask(GL_FALSE) ;

	glCallList(MATERIAL_SPECULAR) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
//			glCallList(ROOF_LIST) ;	// Roof�����ϥϥ��饤�ȿ��򥰥�ǡ������
		glPopMatrix() ;

		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;

		glEnable(GL_CULL_FACE) ;
	}


//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif


	// MATERIAL_MAT_IRON �����å�
//	glCallList(MATERIAL_MAT_SPECULAR) ;
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	// MATERIAL_GOLD �����å�
//	glCallList(MATERIAL_SPECULAR) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glDisable(GL_BLEND) ;
		glDepthMask(GL_TRUE) ;

		return ;
	}


	// MATERIAL_RED_GRAY �����å�
//	glCallList(MATERIAL_MAT_SPECULAR) ;
	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
//		glCallList(MATERIAL_SPECULAR) ;
		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
//		glCallList(MATERIAL_MAT_SPECULAR) ;

		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

/* �����󡢤����̵�������ɤ����ʤ�...
		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;

			glPushAttrib(GL_LIGHTING_BIT) ;
				glCallList(MATERIAL_SEC_SPECULAR) ;

				for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
				{
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;
		}
		// �ÿ˥⡼�����֥顼�¹Ի��⡢�Ǹ�ΰ��������Ǥǥ������ �ϡ��ʤ���
		else
		{
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			glPopMatrix() ;
		}
*/
		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

//#ifdef GL_VERSION_1_1
//		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif

		// ɽ�̥��饹����
		if (clock_glass)
		{
			if (clock_glass == 1)
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;	// -glass 1 �ξ���ž

			glCallList(GLASS_LIST) ;
		}


	glPopMatrix() ;

	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


void DrawEnvSpecularModulateChrome_texture_object(ClockState& st)
{
	glFogfv(GL_FOG_COLOR, currentBG) ;

	// ���������
	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;

//	glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
//	glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
	glColor3fv(currentCL) ;
	glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;
//	glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
	glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;
//	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;

	glDepthMask(GL_FALSE) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glDisable(GL_BLEND) ;
		glDepthMask(GL_TRUE) ;

		return ;
	}


	glEnable(GL_BLEND) ;

	if (!reflection)	// ȿ�ͤʤ���
	{
		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;

		glFogfv(GL_FOG_COLOR, black) ;
		glCallList(ENV_BOARD_LIST) ;
		glFogfv(GL_FOG_COLOR, currentBG) ;

		glPushMatrix() ;
		{
			glDisable(GL_BLEND) ;

				// OpenGL-1.1
//				if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//				glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

				glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
				DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
				glShadeModel(GL_SMOOTH) ;

				// ���׿�����
				glDepthMask(GL_TRUE) ;

				// OpenGL-1.1
				glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;

				glPushMatrix() ;
					glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_HOUR_LIST) ;
				glPopMatrix() ;

				// OpenGL-1.1
				glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;

				glPushMatrix() ;
					glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_MINUTE_LIST) ;
				glPopMatrix() ;

				glColor3fv(currentCL) ;
				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

				if (s_motion && !motion)
				{
					// �ÿˤ����⡼�����֥顼�ν���
					GLfloat sec_angle = st.start_angle ;//, center_angle ;

					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
					{
						glEnable(GL_BLEND) ;
						glDepthMask(GL_FALSE) ;
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

						// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
						// OpenGL-1.1
						glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

						for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
						{
							// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//							if (i == (s_motion + 1) / 2)
//							{
//								center_angle = sec_angle ;
//								continue ;
//							}
							glPushMatrix() ;
							{
								glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_SECOND_LIST) ;
							}
							glPopMatrix() ;
						}
					}
					glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//					glBlendFunc(addSrcBlendFactor, GL_ONE) ;
//#endif

					// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//					glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
//					glPushMatrix() ;
//						glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//						glCallList(ENV_SECOND_LIST) ;
//					glPopMatrix() ;

					// OpenGL-1.1
					glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�

					glPushMatrix() ;
					{
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}
				else
				{
					// OpenGL-1.1
					glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�

					glPushMatrix() ;
					{
						glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}

			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

			glDepthMask(GL_FALSE) ;
			glEnable(GL_BLEND) ;

			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

			// ɽ�̥��饹����
			if (clock_glass)
//			if (!(detail == 1 || detail == 2) && clock_glass)
			{
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;
			}
		}
		glPopMatrix() ;
	}
	else	// -REFLECT ���ץ��������
	{
		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.16f, 3.0f)) ;	// ����Ū�ˤϤ���
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], Pow(ReflectAlphaByCos(zAxis, 0.4f))) ;	// ����Ū�ˤϤ���
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.1f)) ;	// ����Ū�ˤϤ���
		}
		else
		{
			// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			// ADD �Τޤ�
			glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
		}

		glEnable(GL_BLEND) ;

//		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
//		if (fogFlag) glFogfv(GL_FOG_COLOR, black) ;

			// OpenGL-1.1
			glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;


#ifdef CULL_REF_SKY_BY_ANGLE
		if (zAxis > CULL_REF_SKY_AXIS)
		{
			glPushMatrix() ;
			{
				glTranslatef(0.0f, 0.0f, -4.2f) ;
				glCallList(ENV_BOARD_LIST) ;
			}
			glPopMatrix() ;
		}

#else	// #ifdef CULL_REF_SKY_BY_ANGLE

			glPushMatrix() ;
			{
				glTranslatef(0.0f, 0.0f, -3.8f) ;
				glCallList(ENV_BOARD_LIST) ;
			}
			glPopMatrix() ;

#endif	// #ifdef CULL_REF_SKY_BY_ANGLE ... #else


		glPushMatrix() ;
		{
			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;
			{
					glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;

					glColor3f(currentCL[0]*realitySpecularBright*0.4, currentCL[1]*realitySpecularBright*0.4f, currentCL[2]*realitySpecularBright*0.4f) ;
					glFogfv(GL_FOG_COLOR, black) ;
					glCallList(ENV_BOARD_DETAIL_LIST) ;

					// �Ǥ������ʬ����
					if (envOpenGL != VOODOO_MESA_OLD)
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
					else
					// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
						glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;

//					if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;

					// �����Ĥε�����
					float refBright = ReflectBrightByCos(zAxis) ;

					glDepthMask(GL_TRUE) ;
					glFogfv(GL_FOG_COLOR, currentBG) ;
					glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

					// OpenGL-1.1
					DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate, refBright, ReflectAlphaByCos(zAxis, 0.4f)) ;

					glShadeModel(GL_SMOOTH) ;

					glPushMatrix() ;
					{
						glScalef(1.0f, 1.0f, -1.0f) ;
						glFrontFace(GL_CCW) ;

						// ���׿�����
//						glDepthMask(GL_TRUE) ;

						glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;

						glPushMatrix() ;
							glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MIRROR_HOUR_LIST) ;
						glPopMatrix() ;

						glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;

						glPushMatrix() ;
							glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MIRROR_MINUTE_LIST) ;
						glPopMatrix() ;

						glColor3fv(currentCL) ;
						glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

						if (s_motion && !motion)
						{
							// �ÿˤ����⡼�����֥顼�ν���
							GLfloat sec_angle = st.start_angle ;
							GLfloat refAlpha = ReflectAlphaByCos(zAxis, 0.3f) ;

							glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
							{
								glEnable(GL_BLEND) ;
								glDepthMask(GL_FALSE) ;
//								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

								// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
								// OpenGL-1.1
								glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;

								for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
								{
									glPushMatrix() ;
										glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
										glCallList(ENV_MIRROR_SECOND_LIST) ;
									glPopMatrix() ;
								}
							}
							glPopAttrib() ;
						}
						else
						{
							// OpenGL-1.1
							glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�

							glPushMatrix() ;
								glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_MIRROR_SECOND_LIST) ;
							glPopMatrix() ;
						}

						glDepthMask(GL_FALSE) ;
						glFrontFace(GL_CW) ;
					}
					glPopMatrix() ;
			}
			glPopMatrix() ;

//			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;

//#ifdef GL_VERSION_1_1
			// OpenGL-1.1
			if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

			glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;
			glColor3f(currentCL[0]*realitySpecularBright*0.76f, currentCL[1]*realitySpecularBright*0.76f, currentCL[2]*realitySpecularBright*0.76f) ;

			glFogfv(GL_FOG_COLOR, black) ;
			glCallList(ENV_MIRROR_WALL_LIST) ;

			glFrontFace(GL_CCW) ;
		}
		glPopMatrix() ;

		glFogfv(GL_FOG_COLOR, currentBG) ;

		// ��������
		glPushMatrix() ;
		{
			glCallList(ENV_BOARD_DETAIL_LIST) ;

			glPushMatrix() ;
			{
				glPushMatrix() ;
				{
					// ���׿�����
					glDepthMask(GL_TRUE) ;
					glDisable(GL_BLEND) ;

					// �����Ĥε�����
					glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
					DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
					glShadeModel(GL_SMOOTH) ;

					glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
					glPushMatrix() ;
						glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_HOUR_LIST) ;
					glPopMatrix() ;

					glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
					glPushMatrix() ;
						glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_MINUTE_LIST) ;
					glPopMatrix() ;

					glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

					if (s_motion && !motion)
					{
						// �ÿˤ����⡼�����֥顼�ν���
						GLfloat sec_angle = st.start_angle ; //, center_angle ;

						glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
						{
							glEnable(GL_BLEND) ;
							glDepthMask(GL_FALSE) ;
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

							// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
							// OpenGL-1.1
							glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

							for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
							{
								glPushMatrix() ;
								{
									glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
									glCallList(ENV_SECOND_LIST) ;
								}
								glPopMatrix() ;
							}
						}
						glPopAttrib() ;

						// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������

						glPushMatrix() ;
							glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_SECOND_LIST) ;
						glPopMatrix() ;
					}
					else
					{
						glColor3fv(currentCL) ;

						glPushMatrix() ;
							glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_SECOND_LIST) ;
						glPopMatrix() ;
					}

					glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

					// ɽ�̥��饹����
//					if (!(detail == 1 || detail == 2) && clock_glass)
					if (clock_glass)
					{
						glFogfv(GL_FOG_COLOR, black) ;

						glDepthMask(GL_FALSE) ;
						glEnable(GL_BLEND) ;

						glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

						glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
						glCallList(ENV_GLASS_LIST) ;

						glFogfv(GL_FOG_COLOR, currentBG) ;
					}
				}
				glPopMatrix() ;
			}
			glPopMatrix() ;
		}
		glPopMatrix() ;

	}

	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;
}


// Texture Object ��Ȥ��ʤ��Ķ���
void DrawEnvSpecularModulateChrome(ClockState& st)
{
	glFogfv(GL_FOG_COLOR, currentBG) ;

	// ���������
	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;

	// MATERIAL_GOLD �����å�
	glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;

	glMatrixMode(GL_TEXTURE) ;
	glPushMatrix() ;
	{
		glCallList(CHROME_TEXTURE_LIST) ;
		glMatrixMode(GL_MODELVIEW) ;

		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;
//		DrawGoldenObjects(14.6f, n_golden) ;

		glMatrixMode(GL_TEXTURE) ;
	}
	glPopMatrix() ;
	glMatrixMode(GL_MODELVIEW) ;

	glDepthMask(GL_FALSE) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glDisable(GL_BLEND) ;
		glDepthMask(GL_TRUE) ;

		return ;
	}


	glEnable(GL_BLEND) ;

	if (!reflection)	// ȿ�ͤʤ���
	{
		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;

		glFogfv(GL_FOG_COLOR, black) ;
		glCallList(ENV_BOARD_LIST) ;
		glFogfv(GL_FOG_COLOR, currentBG) ;

		glPushMatrix() ;
		{
			glDisable(GL_BLEND) ;

			glMatrixMode(GL_TEXTURE) ;
			glPushMatrix() ;
			{
				glCallList(CHROME_TEXTURE_LIST) ;
				glMatrixMode(GL_MODELVIEW) ;

				// �����Ĥε�����
				glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
				DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
				glShadeModel(GL_SMOOTH) ;

				// ���׿�����
				glDepthMask(GL_TRUE) ;

				glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�

				glPushMatrix() ;
					glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_HOUR_LIST) ;
				glPopMatrix() ;


				glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�

				glPushMatrix() ;
					glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_MINUTE_LIST) ;
				glPopMatrix() ;

				if (s_motion && !motion)
				{
					// �ÿˤ����⡼�����֥顼�ν���
					GLfloat sec_angle = st.start_angle ;//, center_angle ;

					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
					{
						glEnable(GL_BLEND) ;
						glDepthMask(GL_FALSE) ;
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

						// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
						glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B, 1.0f / s_motion * S_MOTION_ALPHA) ;

						for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
						{
							// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//							if (i == (s_motion + 1) / 2)
//							{
//								center_angle = sec_angle ;
//								continue ;
//							}
							glPushMatrix() ;
							{
								glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_SECOND_LIST) ;
							}
							glPopMatrix() ;
						}
					}
					glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//					glBlendFunc(addSrcBlendFactor, GL_ONE) ;
//#endif

					// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//					glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
//					glPushMatrix() ;
//						glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//						glCallList(ENV_SECOND_LIST) ;
//					glPopMatrix() ;

					glColor3f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�

					glPushMatrix() ;
					{
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}
				else
				{
					glColor3f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B) ;	// MATERIAL_MAT_GOLD �����å�

					glPushMatrix() ;
					{
						glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}

				glMatrixMode(GL_TEXTURE) ;
			}

			glPopMatrix() ;
			glMatrixMode(GL_MODELVIEW) ;

			glDepthMask(GL_FALSE) ;
			glEnable(GL_BLEND) ;

//#ifdef WIN32 // FX_GL_BETA_2
			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
//#endif

			// ɽ�̥��饹����
//			if (clock_glass)
			if (!(detail == 1 || detail == 2) && clock_glass)
			{
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;
			}
		}
		glPopMatrix() ;
	}
	else	// -REFLECT ���ץ��������
	{
		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.16f, 3.0f)) ;	// ����Ū�ˤϤ���
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], Pow(ReflectAlphaByCos(zAxis, 0.4f))) ;	// ����Ū�ˤϤ���
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.1f)) ;	// ����Ū�ˤϤ���
		}
		else
		{
			// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			// ADD �Τޤ�
			glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
		}

		glEnable(GL_BLEND) ;

//		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
//		glFogfv(GL_FOG_COLOR, black) ;

		glMatrixMode(GL_TEXTURE) ;
		glPushMatrix() ;
		{
			glCallList(CHROME_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

#ifdef CULL_REF_SKY_BY_ANGLE
		if (zAxis > CULL_REF_SKY_AXIS)
		{
			glPushMatrix() ;
			{
				glTranslatef(0.0f, 0.0f, -4.2f) ;
				glCallList(ENV_BOARD_LIST) ;
			}
			glPopMatrix() ;
		}

#else	// #ifdef CULL_REF_SKY_BY_ANGLE

			glPushMatrix() ;
			{
				glTranslatef(0.0f, 0.0f, -3.8f) ;
				glCallList(ENV_BOARD_LIST) ;
			}
			glPopMatrix() ;

#endif	// #ifdef CULL_REF_SKY_BY_ANGLE ... #else


		glPushMatrix() ;
		{
			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;
			{
//#ifndef GL_VERSION_1_1
//		// OpenGL-1.0
//
//				glMatrixMode(GL_TEXTURE) ;
//				glPushMatrix() ;
//				{
//					glCallList(CHROME_TEXTURE_LIST) ;
//					glMatrixMode(GL_MODELVIEW) ;
//
//#else
//		// OpenGL-1.1
//					glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//#endif

//					glBlendFunc(GL_ONE, GL_ONE) ;
					glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;

					glColor3f(currentCL[0]*realitySpecularBright*0.4, currentCL[1]*realitySpecularBright*0.4f, currentCL[2]*realitySpecularBright*0.4f) ;
					glFogfv(GL_FOG_COLOR, black) ;
					glCallList(ENV_BOARD_DETAIL_LIST) ;

					// �Ǥ������ʬ����
					if (envOpenGL != VOODOO_MESA_OLD)
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
					else
					// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
						glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;


					// �����Ĥε�����
					float refBright = ReflectBrightByCos(zAxis) ;

					glDepthMask(GL_TRUE) ;
					glFogfv(GL_FOG_COLOR, currentBG) ;
					glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

					DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate, refBright, ReflectAlphaByCos(zAxis, 0.4f)) ;

					glShadeModel(GL_SMOOTH) ;

					glPushMatrix() ;
					{
						glScalef(1.0f, 1.0f, -1.0f) ;
						glFrontFace(GL_CCW) ;

						// ���׿�����
//						glDepthMask(GL_TRUE) ;

						glColor4f(currentCL[0]*COLOR_CHROME_HOUR_R * refBright, currentCL[1]*COLOR_CHROME_HOUR_G * refBright, currentCL[2]*COLOR_CHROME_HOUR_B * refBright, ReflectAlphaByCos(zAxis, 0.5f)) ;	// MATERIAL_MAT_METAL_BLUE �����å�

						glPushMatrix() ;
							glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MIRROR_HOUR_LIST) ;
						glPopMatrix() ;

						glColor4f(currentCL[0]*COLOR_CHROME_MINUTE_R * refBright, currentCL[1]*COLOR_CHROME_MINUTE_G * refBright, currentCL[2]*COLOR_CHROME_MINUTE_B * refBright, ReflectAlphaByCos(zAxis, 0.36f)) ;	// MATERIAL_MAT_METAL_GREEN �����å�

						glPushMatrix() ;
							glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MIRROR_MINUTE_LIST) ;
						glPopMatrix() ;

						if (s_motion && !motion)
						{
							// �ÿˤ����⡼�����֥顼�ν���
							GLfloat sec_angle = st.start_angle ;
							GLfloat refAlpha = ReflectAlphaByCos(zAxis, 0.3f) ;

							glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
							{
								glEnable(GL_BLEND) ;
								glDepthMask(GL_FALSE) ;
//								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

								// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
								glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R * refBright, currentCL[1]*COLOR_CHROME_SECOND_G * refBright, currentCL[2]*COLOR_CHROME_SECOND_B * refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;

								for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
								{
									glPushMatrix() ;
										glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
										glCallList(ENV_MIRROR_SECOND_LIST) ;
									glPopMatrix() ;
								}
							}
							glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//						glBlendFunc(GL_ONE, GL_ONE) ;
//#endif
						}
						else
						{
							glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R * refBright, currentCL[1]*COLOR_CHROME_SECOND_G * refBright, currentCL[2]*COLOR_CHROME_SECOND_B * refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�

							glPushMatrix() ;
								glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_MIRROR_SECOND_LIST) ;
							glPopMatrix() ;
						}

						glDepthMask(GL_FALSE) ;
						glFrontFace(GL_CW) ;
					}
					glPopMatrix() ;

//#ifndef GL_VERSION_1_1
//			// OpenGL-1.0
//
//					glMatrixMode(GL_TEXTURE) ;
//				}
//				glPopMatrix() ;
//				glMatrixMode(GL_MODELVIEW) ;
//
//#else
//			// OpenGL-1.1
//				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
//				if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif
			}
			glPopMatrix() ;

				glMatrixMode(GL_TEXTURE) ;
			}
			glPopMatrix() ;
			glMatrixMode(GL_MODELVIEW) ;

//			glBlendFunc(GL_ONE, GL_ONE) ;
			glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;
//			glColor3f(currentCL[0], currentCL[1], currentCL[2]) ;
			glColor3f(currentCL[0]*realitySpecularBright*0.76f, currentCL[1]*realitySpecularBright*0.76f, currentCL[2]*realitySpecularBright*0.76f) ;

//#ifdef GL_VERSION_1_1
			if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif
			glFogfv(GL_FOG_COLOR, black) ;
			glCallList(ENV_MIRROR_WALL_LIST) ;

//#ifdef GL_VERSION_1_1
			if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif
			glFrontFace(GL_CCW) ;
		}
		glPopMatrix() ;

//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) ;
		// �Ǥ������ʬ���轪λ

		glFogfv(GL_FOG_COLOR, currentBG) ;

		// ��������
		glPushMatrix() ;
		{
//			glCallList(BOARD_DETAIL_LIST) ;
			glCallList(ENV_BOARD_DETAIL_LIST) ;
//			glFogfv(GL_FOG_COLOR, currentBG) ;

			glPushMatrix() ;
			{
				glPushMatrix() ;
				{
					// ���׿�����
					glDepthMask(GL_TRUE) ;
					glDisable(GL_BLEND) ;

					glMatrixMode(GL_TEXTURE) ;
					glPushMatrix() ;
					{
						glCallList(CHROME_TEXTURE_LIST) ;
						glMatrixMode(GL_MODELVIEW) ;

						// �����Ĥε�����
						glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

						DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;

						glShadeModel(GL_SMOOTH) ;

						glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�

						glPushMatrix() ;
							glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_HOUR_LIST) ;
						glPopMatrix() ;

						glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�

						glPushMatrix() ;
							glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MINUTE_LIST) ;
						glPopMatrix() ;

						if (s_motion && !motion)
						{
							// �ÿˤ����⡼�����֥顼�ν���
							GLfloat sec_angle = st.start_angle ; //, center_angle ;

							glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
							{
								glEnable(GL_BLEND) ;
								glDepthMask(GL_FALSE) ;
								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

								// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
								glColor4f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B, 1.0f / s_motion * S_MOTION_ALPHA) ;

								for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
								{
//									// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//									if (i == (s_motion + 1) / 2)
//									{
//										center_angle = sec_angle ;
//										continue ;
//									}
									glPushMatrix() ;
									{
										glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
										glCallList(ENV_SECOND_LIST) ;
									}
									glPopMatrix() ;
								}
							}
							glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//							glBlendFunc(GL_ONE, GL_ONE) ;
//							glBlendFunc(addSrcBlendFactor, GL_ONE) ;
//#endif

						// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//							glPushMatrix() ;
//								glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//								glCallList(ENV_SECOND_LIST) ;
//							glPopMatrix() ;
							glPushMatrix() ;
								glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_SECOND_LIST) ;
							glPopMatrix() ;
						}
						else
						{
							glColor3f(currentCL[0]*COLOR_CHROME_SECOND_R, currentCL[1]*COLOR_CHROME_SECOND_G, currentCL[2]*COLOR_CHROME_SECOND_B) ;

							glPushMatrix() ;
								glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_SECOND_LIST) ;
							glPopMatrix() ;
						}

						glMatrixMode(GL_TEXTURE) ;
					}
					glPopMatrix() ;
					glMatrixMode(GL_MODELVIEW) ;

					// ɽ�̥��饹����
					if (!(detail == 1 || detail == 2) && clock_glass)
//					if (clock_glass)
					{
						glFogfv(GL_FOG_COLOR, black) ;

						glDepthMask(GL_FALSE) ;
						glEnable(GL_BLEND) ;

						glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

						glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
						glCallList(ENV_GLASS_LIST) ;

						glFogfv(GL_FOG_COLOR, currentBG) ;
					}
				}
				glPopMatrix() ;
			}
			glPopMatrix() ;
		}
		glPopMatrix() ;

//		glPopMatrix() ;

	}

	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;
}


// ����åɥƥ�������ξ夫�顢�Ķ��ޥåԥ󥰥ϥ��饤�Ȥ�Ťͤ��real ���ץ�����
void DrawEnvSpecularModulateClock(ClockState& st)
{
//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif

	glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;
	glEnable(GL_BLEND) ;
	glDepthMask(GL_FALSE) ;

	// ���ڥ���顼�ϸ����ο��򥹥ڥ���顼�����뤵����Ĵ
	// ADD������Ĵ
	// �ƥ������㥪�֥������Ȳ�ǽ�� ONE_MINUS ���ѻ��ϡ����ѤΥƥ�������ȤʤäƤ��뤿�ᡢ
	// glColor �Ǥϸ������򤽤Τޤ޻���
	if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH && has_texture_object)
	{
		glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
		glColor3fv(currentCL) ;
	}
	else
	{
		// ADD ��
		glCallList(ENV_TEXTURE_LIST) ;
		glColor3f(currentCL[0]*realitySpecularBright, currentCL[1]*realitySpecularBright, currentCL[2]*realitySpecularBright) ;
	}

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// -r 1 �Τߺ�����
	if (mode_real == 1)
	{
		if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH && has_texture_object)
			glColor3fv(currentCL) ;
		else	// ADD ��
			glColor3f(currentCL[0]*realitySpecularBright, currentCL[1]*realitySpecularBright, currentCL[2]*realitySpecularBright) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif

	if (has_texture_object)
		DrawEnvSpecularModulateChrome_texture_object(st) ;
	else
		DrawEnvSpecularModulateChrome(st) ;
}


// �Ķ��ޥåԥ󥰤ξ夫�顢�������ǥ��󥰤�Ťͤ�
void DrawShadeWrapClock(ClockState& st)
{
//#ifdef GL_VERSION_1_1
	// OpenGL-1.1 ��
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif

	glBlendFunc(GL_ONE, GL_ONE) ;

	glEnable(GL_BLEND) ;
	glDepthMask(GL_FALSE) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
	}


//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif


	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glDisable(GL_BLEND) ;
		glDepthMask(GL_TRUE) ;

		return ;
	}


	glCallList(BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;
			for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
			{
				glPushMatrix() ;
					glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
					glCallList(SECOND_E3_LIST) ;
				glPopMatrix() ;
			}
		}
		// �ÿ˥⡼�����֥顼�¹Ի��⡢�Ǹ�ΰ��������Ǥǥ������ �ϡ��ʤ���
		else
		{
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}

//#ifdef GL_VERSION_1_1
//		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glEnable(GL_TEXTURE_2D) ;
			glDisable(GL_LIGHTING) ;

			if (!has_texture_object)
			{
				// �ƥ������㡦���֥������Ȥʤ�
				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
				{
					glCallList(ENV_GLASS_TEXTURE_LIST) ;
					glMatrixMode(GL_MODELVIEW) ;
					glColor3f(currentCL[0]*0.6f, currentCL[1]*0.6f, currentCL[2]*0.6f) ;
					glCallList(ENV_GLASS_LIST) ;

					// �ƥ������㡦���֥������Ȥʤ�
					glMatrixMode(GL_TEXTURE) ;
				}
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;
			}
			else
			{
				// �ƥ������㡦���֥������Ȼ���
//				glCallList(ENV_GLASS_TEXTURE_LIST) ;
				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

				glColor3f(currentCL[0]*0.6f, currentCL[1]*0.6f, currentCL[2]*0.6f) ;
				glCallList(ENV_GLASS_LIST) ;

				// �ƥ������㡦���֥������Ȼ���
				glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
			}
		}

	glPopMatrix() ;

	glDisable(GL_BLEND) ;
	glDepthMask(GL_TRUE) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

//#ifdef GL_VERSION_1_1
//	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif
}


// ��������ʥ��ơ������ǡ�
// ɸ��Υ������ǥ��󥰻���
void DrawStandardClock(ClockState& st)
{
//	glEnable(GL_BLEND) ;
//	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE) ;
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
//	glBlendFunc(GL_ONE, GL_ONE) ;
//	glEnable(GL_POLYGON_SMOOTH) ;
//	glDisable(GL_DEPTH_TEST) ;





	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glShadeModel(GL_SMOOTH) ;
		return ;
	}


	glCallList(BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

		DrawHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glEnable(GL_BLEND) ;
//				glDisable(GL_DEPTH_TEST) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
//					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;

#if defined WIN32 // FX_GL_BETA_2 || defined FX_GL_ALPHA
			glBlendFunc(GL_ONE, GL_ONE) ;
#endif
			// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//			glPushMatrix() ;
//				glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//				glCallList(SECOND_LIST) ;
//			glPopMatrix() ;
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;

		}
		else
		{
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
		{
			// DrawSignAndFPS() ��ǥ��åȤ����֥��ɥե��������ꥹ�ȥ�����ʤ���
			glBlendFunc(GL_ONE, GL_ONE) ;

			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			if (clock_glass == 1)
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;

			glCallList(GLASS_LIST) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// ��������ʥ��ơ������ǡ�
// ɸ������饤�Ȥʤ��ξ�������
void DrawSimpleClock(ClockState& st)
{
//	glShadeModel(GL_SMOOTH) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

//	glFlush() ;


	if (cull_status & CULL_STATUS_SHELL)
	{
		glCallList(SHELL_LIST_CULL) ;
	}
	else
	{
//		for (int i = 0 ; i < 16 ; i ++)
		glCallList(SHELL_LIST) ;
	}

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
//		glShadeModel(GL_SMOOTH) ;
		return ;
	}

//	glFlush() ;

	glCallList(BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

		DrawModulateHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
//		glShadeModel(GL_SMOOTH) ;

		// ���׿�����
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(MINUTE_LIST) ;
		glPopMatrix() ;

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glEnable(GL_BLEND) ;
//				glDisable(GL_DEPTH_TEST) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
//					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;

			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
		{
			glBlendFunc(GL_ONE, GL_ONE) ;

			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			if (clock_glass == 1)
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;
			else
				glColor3f(currentCL[0] * 0.2f, currentCL[1] * 0.2f, currentCL[2] * 0.2f) ;

			glFogfv(GL_FOG_COLOR, black) ;
			glCallList(GLASS_LIST) ;
			glFogfv(GL_FOG_COLOR, currentBG) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// -s 2, 3(13)
// -s 3 �ξ��ϰ���ܤ�����Τߡʥ��饹����ϥ��åȡ�
void DrawSolidShadedClock(ClockState& st)
{
	// �̤Υޥƥꥢ�륻�åȡ�Ʊ���� DIFFUSE, AMBIENT �ʳ��Υޥƥꥢ��δ��Ȥʤ��
//	glDisable(GL_COLOR_MATERIAL) ;
	glCallList(MATERIAL_SHELL_SOLID) ;

	// �ʸ�ϡ�glColor �Τߤǿ������ǽ
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE) ;
	glEnable(GL_COLOR_MATERIAL) ;

	glEnable(GL_TEXTURE_2D) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
	}

	// ���δؿ��ΤϤ���� glCallList(MATERIAL_SHELL_SOLID) ; �ѤߤʤΤǺƻ������ס�ENV_*_LIST ��OK��
//	if (!noCullObj && zAxis > CULL_SHELL_Z_AXIS)
//		glCallList(ENV_SHELL_LIST_CULL) ;
//	else
//		glCallList(ENV_SHELL_LIST) ;

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		glDisable(GL_COLOR_MATERIAL) ;
		glDisable(GL_TEXTURE_2D) ;
		return ;
	}
	

	glCallList(BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

//		DrawHedronsWithColorMaterialDiffuseTest(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		DrawHedronsWithColorMaterialDiffuse(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

//	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE) ;
//	glEnable(GL_COLOR_MATERIAL) ;

		// ���׿�����
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(MINUTE_LIST) ;
		glPopMatrix() ;


		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glEnable(GL_BLEND) ;
//				glDisable(GL_DEPTH_TEST) ;
				glDepthMask(GL_FALSE) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1; i ++, sec_angle += st.step_angle)
				{
//					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(SECOND_LIST) ;
					glPopMatrix() ;
				}
			glPopAttrib() ;

			glBlendFunc(GL_ONE, GL_ONE) ;

			// �ÿ˥⡼�����֥顼�¹Ի��⡢����ΰ��������Ǥǥ������
//			glPushMatrix() ;
//				glRotatef(center_angle, 0.0f,0.0f,-1.0f) ;
//				glCallList(SECOND_LIST) ;
//			glPopMatrix() ;
			glPushMatrix() ;
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}
		else
		{
			glPushMatrix() ;
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(SECOND_LIST) ;
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
//		if (clock_glass && !(solid && texture == 3))
		if (clock_glass && mode_solid != 3)
		{
			// DrawSignAndFPS() ��ǥ��åȤ����֥��ɥե��������ꥹ�ȥ�����ʤ���
			glBlendFunc(GL_ONE, GL_ONE) ;

			glDisable(GL_TEXTURE_2D) ;
			glDisable(GL_COLOR_MATERIAL) ;

			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;

			if (clock_glass == 1)
				glRotatef(st.g_spin, 0.0f,0.0f,1.0f) ;	// -glass 1 �ξ���ž

			glCallList(GLASS_LIST) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}
		else
		{
			glDisable(GL_TEXTURE_2D) ;
			glDisable(GL_COLOR_MATERIAL) ;
		}

	glPopMatrix() ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
}


// Z�Хåե�������
void DrawDepthClock(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	if (!(mode_solid == 1 && filterTest))
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	glCallList(ENV_BOARD_LIST) ;

	glPushMatrix() ;

		// �����Ĥε�����
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;

		// ���׿�����
		glPushMatrix() ;
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		glPopMatrix() ;

		glPushMatrix() ;
			glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_SECOND_LIST) ;
		glPopMatrix() ;

	glPopMatrix() ;
}


// ���������-real 1 ���ץ����ΰ���ܤ������
// �ƥ������㡦���֥������Ȼ���
void DrawSolidModulateClock_texture_object(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;

			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;

		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	glColor3f(currentCL[0], currentCL[1], currentCL[2]*0.8f) ;
//	glColor3fv(currentCL) ;

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	if (!reflection)	// ȿ�ͤʤ���
	{
		glColor4f(currentCL[0]*1.0f, currentCL[1]*0.7f, currentCL[2]*0.7f, 0.2f) ;

//		glCallList(SOLID2_TEXTURE_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;

		glCallList(ENV_BOARD_LIST) ;

//		glCallList(SOLID_TEXTURE_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
	}
	else	// -R ���ץ��������
	{
		glDepthMask(GL_FALSE) ;
		// MATERIAL_RED_GRAY �����å�
		glColor4f(currentCL[0], currentCL[1]*0.7f, currentCL[2]*0.7f, 0.2f) ;

//		glCallList(SOLID2_TEXTURE_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;

		glCallList(ENV_BOARD_LIST) ;

//		glCallList(SOLID_TEXTURE_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;

		glDepthMask(GL_TRUE) ;

		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		else
		// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;

		glEnable(GL_BLEND) ;

		glPushMatrix() ;
		{
			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;
			{
				glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
//				glFogfv(GL_FOG_COLOR, white) ;	// �ʤ����ե������顼�������ˤʤ롪��
				glCallList(ENV_BOARD_DETAIL_LIST) ;
//				glFogfv(GL_FOG_COLOR, currentBG) ;
//				glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
			}
			glPopMatrix() ;

			glColor4f(currentCL[0], currentCL[1], currentCL[2], 0.4f) ;
			glCallList(ENV_MIRROR_WALL_LIST) ;

			glFrontFace(GL_CCW) ;
		}
		glPopMatrix() ;

		glBlendFunc(GL_ONE, GL_ONE) ;//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) ;
		glDisable(GL_BLEND) ;
		// �Ǥ������ʬ���轪λ

		// ��������
		glPushMatrix() ;
			glColor3f(currentCL[0], currentCL[1]*0.8f, currentCL[2]*0.8f) ;
			glCallList(BOARD_DETAIL_LIST) ;
		glPopMatrix() ;
	}
}


// ���������-real 1 ���ץ����ΰ���ܤ������
void DrawSolidModulateClock(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;

			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;

		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	// MATERIAL_MAT_IRON �����å�
	glColor3f(currentCL[0], currentCL[1], currentCL[2]*0.8f) ;
//	glColor3fv(currentCL) ;

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}


	if (!reflection)	// ȿ�ͤʤ���
	{
		glMatrixMode(GL_TEXTURE) ;
		// MATERIAL_RED_GRAY �����å�
		glColor4f(currentCL[0]*1.0f, currentCL[1]*0.7f, currentCL[2]*0.7f, 0.2f) ;
		glPushMatrix() ;
			glCallList(SOLID2_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

//			glCallList(BOARD_LIST) ;
			glCallList(ENV_BOARD_LIST) ;

			glMatrixMode(GL_TEXTURE) ;
		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
	}
	else	// -R ���ץ��������
	{
		glDepthMask(GL_FALSE) ;
		// MATERIAL_RED_GRAY �����å�
		glColor4f(currentCL[0], currentCL[1]*0.7f, currentCL[2]*0.7f, 0.2f) ;

		glMatrixMode(GL_TEXTURE) ;
		glPushMatrix() ;
			glCallList(SOLID2_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

			glCallList(ENV_BOARD_LIST) ;

			glMatrixMode(GL_TEXTURE) ;
		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
//		glCallList(ENV_BOARD_LIST) ;

		glDepthMask(GL_TRUE) ;

		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		else
		// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;

		glEnable(GL_BLEND) ;

		glPushMatrix() ;
		{
			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;
			{
				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
					glCallList(SOLID_TEXTURE_LIST) ;
					glMatrixMode(GL_MODELVIEW) ;
//				glFogfv(GL_FOG_COLOR, white) ;	// �ʤ����ե������顼�������ˤʤ롪��
						glCallList(ENV_BOARD_DETAIL_LIST) ;
//				glFogfv(GL_FOG_COLOR, currentBG) ;
					glMatrixMode(GL_TEXTURE) ;
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;
			}
			glPopMatrix() ;

			glColor4f(currentCL[0], currentCL[1], currentCL[2], 0.4f) ;
			glCallList(ENV_MIRROR_WALL_LIST) ;

			glFrontFace(GL_CCW) ;
		}
		glPopMatrix() ;

		glBlendFunc(GL_ONE, GL_ONE) ;//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) ;
		glDisable(GL_BLEND) ;
		// �Ǥ������ʬ���轪λ

		// ��������
		glPushMatrix() ;
			glColor3f(currentCL[0], currentCL[1]*0.8f, currentCL[2]*0.8f) ;
			glCallList(BOARD_DETAIL_LIST) ;
		glPopMatrix() ;
	}
}



//#ifdef GL_VERSION_1_1
// OpenGL-1.0 �Ǥ�������뤳�ȤϤʤ�����
// �ƥ������㥪�֥������Ȥδؿ���ȤäƤ��뤿��


void DrawDetailModulateShell(ClockState& st)
{
	glCallList(DETAIL_TEXTURE_LIST) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE) ;
	glEnable(GL_TEXTURE_2D) ;

	// �饤�ƥ��� OFF
	glDisable(GL_LIGHTING) ;

	// �֥��ǥ��󥰥⡼�ɥ��å�
	glBlendFunc(multiSrcBlendFactor, multiDstBlendFactor) ;
	glEnable(GL_BLEND) ;

	if (fogFlag)
		glDisable(GL_FOG) ;

	glDepthFunc(GL_LEQUAL) ;
	glDepthMask(GL_FALSE) ;

//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

	// ������
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;

			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_DETAILTEX_LIST) ;

		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_DETAILTEX_LIST_CULL) ;
	else
		glCallList(SHELL_DETAILTEX_LIST) ;

//	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;


	// ����ʹߤ�ɽ�����Τ�
	if (!(cull_status & CULL_STATUS_FRONT))
	{
		// �ܡ���
		glCallList(BOARD_DETAILTEX_LIST) ;

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	}
	else
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	}

//	glDisable(GL_BLEND) ;
//	glDisable(GL_TEXTURE_2D) ;

	if (fogFlag)
		glEnable(GL_FOG) ;

	glDepthMask(GL_TRUE) ;
//	glDepthFunc(GL_LESS) ;
}


void DrawDetailTextureShell(ClockState& st)
{
	// �ƥ������㥻�å�
	if (detail >= 2)
	{
		glActiveTexture(GL_TEXTURE0) ;
	}

	glCallList(DETAIL_TEXTURE_LIST) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE) ;
	glEnable(GL_TEXTURE_2D) ;

	// �饤�ƥ��� OFF
	glDisable(GL_LIGHTING) ;

	// �֥��ǥ��󥰥⡼�ɥ��å�
	glBlendFunc(multiSrcBlendFactor, multiDstBlendFactor) ;
	glEnable(GL_BLEND) ;

	if (fogFlag)
		glDisable(GL_FOG) ;

	glDepthFunc(GL_LEQUAL) ;

//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

	// ������
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;

			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_DETAILTEX_LIST) ;

		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

//#ifdef GL_VERSION_1_1
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_DETAILTEX_LIST_CULL) ;
	else
		glCallList(SHELL_DETAILTEX_LIST) ;

//	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;


	// ����ʹߤ�ɽ�����Τ�
	if (!(cull_status & CULL_STATUS_FRONT))
	{
		// �ܡ���
		glCallList(BOARD_DETAILTEX_LIST) ;

		glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;

//#ifdef GL_VERSION_1_1
		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 0.0f, 1.2f) ;	// �����˰�ư

			// ɽ�̥��饹����
			if (clock_glass)
			{
				if (fogFlag)
					glEnable(GL_FOG) ;

				glFogfv(GL_FOG_COLOR, black) ;
//				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
				glCallList(ENV_TEXTURE_LIST) ;

				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;
				glFogfv(GL_FOG_COLOR, currentBG) ;
			}
		}
		glPopMatrix() ;
	}
	else
	{
//#ifdef GL_VERSION_1_1
		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif	// #ifdef GL_VERSION_1_1
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	}

	glDisable(GL_BLEND) ;
	glDisable(GL_TEXTURE_2D) ;

	if (fogFlag != FOG_NONE)
		glEnable(GL_FOG) ;

	glDepthFunc(GL_LESS) ;
}


// �ǥץ��ޥ����Ϥ������������ GL_FALSE �˥��åȤ��Ƥ���
// -r 2 �� -R �����ꤵ��Ƥ��ʤ����ϥܡ��ɤΥ��ڥ���顼��Ʊ�ͤ˲û�
void DrawSpecularShell_detail2(ClockState& st, int boardFlag = FALSE)
{
	glDepthFunc(GL_LEQUAL) ;

	glFogfv(GL_FOG_COLOR, black) ;
	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;

	glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;
	glEnable(GL_BLEND) ;
//	glDepthMask(GL_FALSE) ;

	// ���ڥ���顼�ϸ����ο��򥹥ڥ���顼�����뤵����Ĵ
	// ADD������Ĵ
	// �ƥ������㥪�֥������Ȳ�ǽ�� ONE_MINUS ���ѻ��ϡ����ѤΥƥ�������ȤʤäƤ��뤿�ᡢ
	// glColor �Ǥϸ������򤽤Τޤ޻���
	if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH && has_texture_object)
	{
		glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;	// ����ǣϣˡ�	-NO_COLOR ��ɬ�פ���
		glColor3fv(currentCL) ;
	}
	else
	{
		// ADD ��
		glCallList(ENV_TEXTURE_LIST) ;
		glColor3f(currentCL[0]*realitySpecularBright, currentCL[1]*realitySpecularBright, currentCL[2]*realitySpecularBright) ;
	}

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ENV_ROOF_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

/*
	// -r 1 �Τߺ�����
	if (mode_real == 1)
	{
		if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH && has_texture_object)
			glColor3fv(currentCL) ;
		else	// ADD ��
			glColor3f(currentCL[0]*realitySpecularBright, currentCL[1]*realitySpecularBright, currentCL[2]*realitySpecularBright) ;
	}
*/

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(ENV_SHELL_LIST_CULL) ;
	else
		glCallList(ENV_SHELL_LIST) ;

	if (boardFlag)
	{
		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
		glCallList(ENV_BOARD_LIST) ;
	}

	if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	glDepthFunc(GL_LESS) ;
}


void DrawMultiTextureShadingClock_multitexture_DrawBoard(ClockState& st)
{
	glActiveTexture(GL_TEXTURE0) ;
	glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;
	glCallList(BOARD_LIST) ;

	// �饤�ƥ��� OFF
	glDisable(GL_LIGHTING) ;

	// �ƥ������㣱��̵����
	glActiveTexture(GL_TEXTURE1) ;
	glDisable(GL_TEXTURE_2D) ;

	// �ƥ������㣱��̵����
//	glActiveTexture(GL_TEXTURE1) ;
	glDisable(GL_TEXTURE_2D) ;

	if (detail == 3)
	{
		// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
		// �ƥ������㣲��̵����
		glActiveTexture(GL_TEXTURE2) ;
		glDisable(GL_TEXTURE_2D) ;
	}
	else if (detail == 2)
	{
		// �ǥ��ƥ����룲�ξ��ϡ������̡��ܡ��ɤΥ��ڥ���顼��û�
		glDepthMask(GL_FALSE) ;
		glActiveTexture(GL_TEXTURE0) ;
		DrawSpecularShell_detail2(st, TRUE) ;	// �ܡ��ɤ�����
		glDepthMask(GL_TRUE) ;
		glDisable(GL_BLEND) ;
	}

	// �ƥ������㣰�϶�°����⡼�ɤ�
	glActiveTexture(GL_TEXTURE0) ;
	glCallList(GOLD_TEXTURE_LIST) ;

	// ���������
	glColor3fv(currentCL) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//	glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;	// �⿧�ʳ��ϥƥ��������ڤ��ؤ�

	// �����׾奪�֥�����������
	glPushMatrix() ;
	{
		// �����Ĥ�¿��������
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
//		DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����

		// ��
//		glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		}
		glPopMatrix() ;

		// ʬ
//		glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		}
		glPopMatrix() ;

		// ��
		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_BLEND) ;
			{
				glEnable(GL_BLEND) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
				glDepthMask(GL_FALSE) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//				glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, 1.0f / s_motion * S_MOTION_ALPHA) ;
				glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
				{
					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
					{
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}
			}
			glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//			glBlendFunc(GL_ONE, GL_ONE) ;
//			glDisable(GL_BLEND) ;
//#endif

//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glColor3f(currentCL[0], currentCL[1], currentCL[2]) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
			{
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			}
			glPopMatrix() ;
		}
		else	// �ÿ˥⡼�����֥顼�ʤ�
		{
//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glColor3f(currentCL[0], currentCL[1], currentCL[2]) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
			{
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			}
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
		if (clock_glass)
//		if (!(detail == 1 || detail == 2) && clock_glass)
		{
			glFogfv(GL_FOG_COLOR, black) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

			glDepthMask(GL_FALSE) ;
			glEnable(GL_BLEND) ;
//			glBlendFunc(GL_ONE, GL_ONE) ;
			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

//			glColor3f(currentCL[0]*0.7f, currentCL[1]*0.7f, currentCL[2]*0.7f) ;
			glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
			glCallList(ENV_GLASS_LIST) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;

			glFogfv(GL_FOG_COLOR, currentBG) ;
		}

	}
	glPopMatrix() ;
}


void DrawMultiTextureShadingClock_multitexture_DrawBoardWithReflection(ClockState& st)
{
	// �ǥץ��񤭹���̵���ǥܡ�������
	glDepthMask(GL_FALSE) ;

	glActiveTexture(GL_TEXTURE0) ;
	glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;

	// �ǥ��ƥ�����������ϡ��ƥ������㣱����ѤΤޤޥܡ�������
	if (detail < 3)
	{
		if (detail)	// detail == 2
		{
			glCallList(BOARD_LIST) ;

			glDisable(GL_LIGHTING) ;

			// �ǥ��ƥ����룲�ξ��ϡ��ܡ�������塢�����̤Υ��ڥ���顼��֥���
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;
//			glDepthMask(GL_FALSE) ;
			glActiveTexture(GL_TEXTURE0) ;
			DrawSpecularShell_detail2(st) ;

			// ���Τޤޤǣϣ�
//			glDepthMask(GL_TRUE) ;
//			glDisable(GL_BLEND) ;

			// �ƥ��������˥åȣ��򥽥�åɥ⡼�ɤ��᤹
//			glActiveTexture(GL_TEXTURE0) ;
//			glEnable(GL_TEXTURE_2D) ;
			glCallList(SOLID_TEXTURE_LIST) ;
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;

			glEnable(GL_LIGHTING) ;

			// �ƥ��������˥åȣ��򥹥ڥ���顼�⡼�ɤ�
			glActiveTexture(GL_TEXTURE1) ;
			glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;
			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;

			// ���λ����ǥƥ������㹽���� !detail ��Ʊ��
		}
		else
		{
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;
			glCallList(BOARD_LIST) ;
		}

		// �ƥ������㣱 ON
		glEnable(GL_TEXTURE_2D) ;
	}
	else if (detail == 3)
	{
		// �ƥ������㣲��̵����
		glActiveTexture(GL_TEXTURE2) ;
		glDisable(GL_TEXTURE_2D) ;
		glCallList(BOARD_LIST) ;

		// �ƥ��������˥åȣ��򥹥ڥ���顼�⡼�ɤ�
		glActiveTexture(GL_TEXTURE1) ;
		glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;

		// ���λ����ǥƥ������㹽���� !detail ��Ʊ��
	}

	glDepthMask(GL_TRUE) ;


	// ��ɸȿž
	glPushMatrix() ;
	{
		// �Ǥꤳ���������
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		glEnable(GL_BLEND) ;
		glScalef(1.0f, 1.0f, -1.0f) ;
		glFrontFace(GL_CW) ;

		// �Ǥꤳ�ߥǥ��ƥ���������
		glPushMatrix() ;
		{
			glActiveTexture(GL_TEXTURE0) ;
			glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
			glCallList(MATERIAL_MIRROR_RED_GRAY) ;
			glCallList(ENV_BOARD_DETAIL_LIST) ;		// �ޥƥꥢ��ϥ��åȺѤߤΤ��� ENV_
		}
		glPopMatrix() ;

		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		// �ƥ������㣱 OFF
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ƥ������㣰�϶�°����⡼�ɤ�
		glActiveTexture(GL_TEXTURE0) ;
		glCallList(CHROME_TEXTURE_LIST) ;

		// ��ž���֥������ȡ������ζ�°���֥������ȱǤꤳ������

		glPushMatrix() ;
		{
			float refBright = ReflectBrightByCos(zAxis) ;

			// �����Ĥε�����
			glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

			// OpenGL-1.1
			DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate, refBright, ReflectAlphaByCos(zAxis, 0.4f)) ;

			glShadeModel(GL_SMOOTH) ;

			glPushMatrix() ;
			{
				glScalef(1.0f, 1.0f, -1.0f) ;
				glFrontFace(GL_CCW) ;

				// ���׿�����
//				glDepthMask(GL_TRUE) ;

//				glColor4f(currentCL[0]*COLOR_CHROME_HOUR_R * refBright, currentCL[1]*COLOR_CHROME_HOUR_G * refBright, currentCL[2]*COLOR_CHROME_HOUR_B * refBright, ReflectAlphaByCos(zAxis, 0.5f)) ;	// MATERIAL_MAT_METAL_BLUE �����å�
				glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
				glPushMatrix() ;
					glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_MIRROR_HOUR_LIST) ;
				glPopMatrix() ;

//				glColor4f(currentCL[0]*COLOR_CHROME_MINUTE_R * refBright, currentCL[1]*COLOR_CHROME_MINUTE_G * refBright, currentCL[2]*COLOR_CHROME_MINUTE_B * refBright, ReflectAlphaByCos(zAxis, 0.36f)) ;	// MATERIAL_MAT_METAL_GREEN �����å�
				glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
				glPushMatrix() ;
					glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_MIRROR_MINUTE_LIST) ;
				glPopMatrix() ;


				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

				if (s_motion && !motion)
				{
					// �ÿˤ����⡼�����֥顼�ν���
					GLfloat sec_angle = st.start_angle ;
					GLfloat refAlpha = ReflectAlphaByCos(zAxis, 0.3f) ;

					glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
					{
//						glEnable(GL_BLEND) ;
						glDepthMask(GL_FALSE) ;
//						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

						// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//						glColor4f(currentCL[0]*refBright, currentCL[1]*0.75f*refBright, currentCL[2]*0.05f*refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;
						glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;

						for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
						{
							if (i == s_motion - 1) glDepthMask(GL_TRUE) ;

							glPushMatrix() ;
								glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
								glCallList(ENV_MIRROR_SECOND_LIST) ;
							glPopMatrix() ;
						}
					}
					glPopAttrib() ;
				}
				else
				{
//					glColor4f(currentCL[0]*refBright, currentCL[1]*0.75f*refBright, currentCL[2]*0.05f*refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�
					glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�
					glPushMatrix() ;
						glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_MIRROR_SECOND_LIST) ;
					glPopMatrix() ;
				}

//				glDepthMask(GL_FALSE) ;
				glFrontFace(GL_CW) ;
			}
			glPopMatrix() ;

		}
		glPopMatrix() ;


		// ���αǤ��������
//		glBlendFunc(GL_ONE, GL_ONE) ;
//		glActiveTexture(GL_TEXTURE0) ;
//		glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
//		glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
//		glFogfv(GL_FOG_COLOR, black) ;

		// 99-09-23
		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
		glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.16f, 3.0f)) ;
//		glColor4f(currentCL[0], currentCL[1], currentCL[2], Pow(ReflectAlphaByCos(zAxis, 0.4f))) ;


#ifdef CULL_REF_SKY_BY_ANGLE

		if (zAxis > CULL_REF_SKY_AXIS)
		{
			glPushMatrix() ;
			{
				glScalef(1.0f, 1.0f, -1.0f) ;
				glTranslatef(0.0f, 0.0f, -4.2f) ;
				glFrontFace(GL_CCW) ;
				glCallList(ENV_BOARD_LIST) ;
				glFrontFace(GL_CW) ;
			}
			glPopMatrix() ;
		}

#else	// #ifdef CULL_REF_SKY_BY_ANGLE

		glPushMatrix() ;
		{
			glScalef(1.0f, 1.0f, -1.0f) ;
			glTranslatef(0.0f, 0.0f, -3.8f) ;
			glFrontFace(GL_CCW) ;
			glCallList(ENV_BOARD_LIST) ;
			glFrontFace(GL_CW) ;
		}
		glPopMatrix() ;

#endif	// #ifdef CULL_REF_SKY_BY_ANGLE ... #else


//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
//		glFogfv(GL_FOG_COLOR, currentBG) ;

		// �Ǥꤳ��������
		// �饤�ƥ��� ON
		glEnable(GL_LIGHTING) ;

		// �ƥ������㣰����åɥ⡼��
//		glActiveTexture(GL_TEXTURE0) ;
		glCallList(SOLID_TEXTURE_LIST) ;
//		glEnable(GL_TEXTURE_2D) ;

		// �ƥ������㣱���ڥ���顼�⡼��
		glActiveTexture(GL_TEXTURE1) ;
		glEnable(GL_TEXTURE_2D) ;
		glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;

		glCallList(MIRROR_WALL_LIST) ;

		// �Ǥ���߽�λ
		glFrontFace(GL_CCW) ;
		glDisable(GL_BLEND) ;
	}
	glPopMatrix() ;


	// ���Υǥ��ƥ�����
	glPushMatrix() ;
	{
		glCallList(BOARD_DETAIL_LIST) ;
	}
	glPopMatrix() ;


	// ���ζ�°��ʬ����
	// �饤�ƥ��� OFF
	glDisable(GL_LIGHTING) ;

	// �ƥ������㣱 OFF
	glActiveTexture(GL_TEXTURE1) ;
	glDisable(GL_TEXTURE_2D) ;

	// �ƥ������㣰�϶�°����⡼�ɤ�
	glActiveTexture(GL_TEXTURE0) ;
//	glCallList(CHROME_TEXTURE_LIST) ;
	glCallList(GOLD_TEXTURE_LIST) ;

	// ���������
	// MATERIAL_GOLD �����å�
//	glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
	glColor3fv(currentCL) ;
	glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//	glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

	// �����׾奪�֥�����������
	glPushMatrix() ;
	{
		// �����Ĥ�¿��������
		glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
		// OpenGL-1.1
		DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
		glShadeModel(GL_SMOOTH) ;

		// ���׿�����

		// ��
//		glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
		glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_HOUR_LIST) ;
		}
		glPopMatrix() ;

		// ʬ
//		glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
		glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
		glPushMatrix() ;
		{
			glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
			glCallList(ENV_MINUTE_LIST) ;
		}
		glPopMatrix() ;


		// ��
		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

		if (s_motion && !motion)
		{
			// �ÿˤ����⡼�����֥顼�ν���
			GLfloat sec_angle = st.start_angle ;//, center_angle ;

			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_BLEND) ;
			{
				glEnable(GL_BLEND) ;
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
				glDepthMask(GL_FALSE) ;

				// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//				glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, 1.0f / s_motion * S_MOTION_ALPHA) ;
				glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

				for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
				{
					// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//					if (i == (s_motion + 1) / 2)
//					{
//						center_angle = sec_angle ;
//						continue ;
//					}
					glPushMatrix() ;
					{
						glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_SECOND_LIST) ;
					}
					glPopMatrix() ;
				}
			}
			glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//			glBlendFunc(GL_ONE, GL_ONE) ;
//			glDisable(GL_BLEND) ;
//#endif

//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
			{
				glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			}
			glPopMatrix() ;
		}
		else	// �ÿ˥⡼�����֥顼�ʤ�
		{
//			glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
			glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�
			glPushMatrix() ;
			{
				glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_SECOND_LIST) ;
			}
			glPopMatrix() ;
		}

		// ɽ�̥��饹����
//		if (!(detail == 1 || detail == 2) && clock_glass)
		if (clock_glass)
		{
			glFogfv(GL_FOG_COLOR, black) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

			glDepthMask(GL_FALSE) ;
			glEnable(GL_BLEND) ;
//			glBlendFunc(GL_ONE, GL_ONE) ;
			glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

//			glColor3f(currentCL[0]*0.7f, currentCL[1]*0.7f, currentCL[2]*0.7f) ;
			glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
			glCallList(ENV_GLASS_LIST) ;

			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;

			glFogfv(GL_FOG_COLOR, currentBG) ;
		}

	}
	glPopMatrix() ;
}


void DrawMultiTextureShadingClock_multitexture(ClockState& st)
{
	// �ޥ���ƥ������㥻�åƥ���
	// ����å���
	// �ƥ������㣰�˥���åɥƥ�������
	glActiveTexture(GL_TEXTURE0) ;
	glCallList(SOLID_TEXTURE_LIST) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	glEnable(GL_TEXTURE_2D) ;

	if (detail)	// detail == 2 || detail == 3)
	{
		if (detail == 3)
		{
			// ���ѥ����ƥ�������ʥ��ڥ���顼�ޤ�Ʊ��������ˤ���ǽ�ʾ��
			// �ƥ������㣲�˥ϥ��饤�ȥƥ�������
			glActiveTexture(GL_TEXTURE2) ;
			glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
			// ������ bind ���줿�ƥ��������SPECULAR_MULTI_TEXTURE_NAME�ˤ�
			// GL_TEXTURE_ENV_COLOR �� �����ȥϥ��饤�ȥޥåץ��顼������
			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;
			glEnable(GL_TEXTURE_2D) ;
		}

		// �ƥ������㣱�˥ǥ��ƥ�����ƥ�������
		// ��ǥ�졼�ȥ֥���
		glActiveTexture(GL_TEXTURE1) ;
		glCallList(DETAIL_TEXTURE_LIST) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
		glEnable(GL_TEXTURE_2D) ;
	}
	else
	{
		// �ǥ��ƥ�����ʤ����̾������

		// �ƥ������㣱�˥ϥ��饤�ȥƥ�������
		glActiveTexture(GL_TEXTURE1) ;
		glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
		// ������ bind ���줿�ƥ��������SPECULAR_MULTI_TEXTURE_NAME�ˤ�
		// GL_TEXTURE_ENV_COLOR �� �����ȥϥ��饤�ȥޥåץ��顼������
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;
		glEnable(GL_TEXTURE_2D) ;
	}

	// �饤�ƥ��� ON
	glEnable(GL_LIGHTING) ;

	// ������
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
		glEnable(GL_CULL_FACE) ;
	}


	// ��������
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		// �����Τ����褷�� return

		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		// �ƥ������㣱��̵����
//		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
		if (detail == 3)
		{
			// �ƥ������㣲��̵����
			glActiveTexture(GL_TEXTURE2) ;
			glDisable(GL_TEXTURE_2D) ;
		}
		else if (detail == 2)
		{
			// �ǥ��ƥ����룲�ξ��ϡ������̤Υ��ڥ���顼��û�
			glDepthMask(GL_FALSE) ;
			glActiveTexture(GL_TEXTURE0) ;
			DrawSpecularShell_detail2(st) ;
			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;
		}

		// �ƥ������㣰�϶�°����⡼�ɤ�
		glActiveTexture(GL_TEXTURE0) ;
//		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
		glCallList(GOLD_TEXTURE_LIST) ;

		// ���������
		// MATERIAL_GOLD �����å�
//		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
		glColor3fv(currentCL) ;
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//		glDisable(GL_TEXTURE_2D) ;
		glActiveTexture(GL_TEXTURE0) ;
		glDisable(GL_TEXTURE_2D) ;

		return ;
	}


	// ����ʸ��������
	if (!reflection)	// �Ǥꤳ�ߤʤ��⡼��
	{
		DrawMultiTextureShadingClock_multitexture_DrawBoard(st) ;
	}
	else	// -REFLECT ���ץ��������
	{
		DrawMultiTextureShadingClock_multitexture_DrawBoardWithReflection(st) ;
	}

	glActiveTexture(GL_TEXTURE0) ;
	glDisable(GL_TEXTURE_2D) ;
}


/*
void DrawMultiTextureShadingClock_multitexture(ClockState& st)
{
	// �ޥ���ƥ������㥻�åƥ���
	// ����å���
	// �ƥ������㣰�˥���åɥƥ�������
	glActiveTexture(GL_TEXTURE0) ;
	glCallList(SOLID_TEXTURE_LIST) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
	glEnable(GL_TEXTURE_2D) ;

	// �ƥ������㣱�˥ϥ��饤�ȥƥ�������
	glActiveTexture(GL_TEXTURE1) ;
	glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
	// ������ bind ���줿�ƥ��������SPECULAR_MULTI_TEXTURE_NAME�ˤ�
	// GL_TEXTURE_ENV_COLOR �� �����ȥϥ��饤�ȥޥåץ��顼������
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;
	glEnable(GL_TEXTURE_2D) ;

	// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
	if (detail == 3)
	{
		// �ƥ������㣲�˥ǥ��ƥ�����ƥ�������
		// �ǽ���̤򤵤�˥�ǥ�졼��
		glActiveTexture(GL_TEXTURE2) ;
		glCallList(DETAIL_TEXTURE_LIST) ;
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
		glEnable(GL_TEXTURE_2D) ;
	}

	// �饤�ƥ��� ON
	glEnable(GL_LIGHTING) ;

	// ������
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
		glEnable(GL_CULL_FACE) ;
	}


	// ��������
	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		// �����Τ����褷�� return

		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		// �ƥ������㣱��̵����
//		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
		if (detail == 3)
		{
			// �ƥ������㣲��̵����
			glActiveTexture(GL_TEXTURE2) ;
			glDisable(GL_TEXTURE_2D) ;
		}

		// �ƥ������㣰�϶�°����⡼�ɤ�
		glActiveTexture(GL_TEXTURE0) ;
//		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glCallList(CHROME_TEXTURE_LIST) ;
		glCallList(GOLD_TEXTURE_LIST) ;

		// ���������
		// MATERIAL_GOLD �����å�
//		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
		glColor3fv(currentCL) ;
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//		glDisable(GL_TEXTURE_2D) ;
		glActiveTexture(GL_TEXTURE0) ;
		glDisable(GL_TEXTURE_2D) ;

		return ;
	}


	// ����ʸ��������
	if (!reflection)	// �Ǥꤳ�ߤʤ��⡼��
	{
		// OpenGL-1.1
		glActiveTexture(GL_TEXTURE0) ;
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;

		// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
		// �ܤ���ߤʤ��⡼�ɤǤϡ�
		// ������䳸��Ʊ���������ƥ����������
		glCallList(BOARD_LIST) ;

		if (detail == 3)
		{
			// �ƥ������㣲��̵����
			glActiveTexture(GL_TEXTURE2) ;
			glDisable(GL_TEXTURE_2D) ;
		}

		// ��°��ʬ����

		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		// �ƥ������㣱��̵����
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ƥ������㣰�϶�°����⡼�ɤ�
		glActiveTexture(GL_TEXTURE0) ;
//		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
//		glCallList(CHROME_TEXTURE_LIST) ;
		glCallList(GOLD_TEXTURE_LIST) ;

		// ���������
		// MATERIAL_GOLD �����å�
//		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
		glColor3fv(currentCL) ;
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;	// �⿧�ʳ��ϥƥ��������ڤ��ؤ�

		// �����׾奪�֥�����������
		glPushMatrix() ;
		{
			// �����Ĥ�¿��������
			glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
//			DrawMirrorHedrons(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
			DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
			glShadeModel(GL_SMOOTH) ;

			// ���׿�����

			// ��
//			glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
			glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
			glPushMatrix() ;
			{
				glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_HOUR_LIST) ;
			}
			glPopMatrix() ;

			// ʬ
//			glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
			glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
			glPushMatrix() ;
			{
				glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_MINUTE_LIST) ;
			}
			glPopMatrix() ;

			// ��
			glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

			if (s_motion && !motion)
			{
				// �ÿˤ����⡼�����֥顼�ν���
				GLfloat sec_angle = st.start_angle ;//, center_angle ;

				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_BLEND) ;
				{
					glEnable(GL_BLEND) ;
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
					glDepthMask(GL_FALSE) ;

					// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//					glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, 1.0f / s_motion * S_MOTION_ALPHA) ;
					glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

					for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
					{
						// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//						if (i == (s_motion + 1) / 2)
//						{
//							center_angle = sec_angle ;
//							continue ;
//						}
						glPushMatrix() ;
						{
							glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_SECOND_LIST) ;
						}
						glPopMatrix() ;
					}
				}
				glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//				glBlendFunc(GL_ONE, GL_ONE) ;
//				glDisable(GL_BLEND) ;
//#endif

//				glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
				glColor3f(currentCL[0], currentCL[1], currentCL[2]) ;	// MATERIAL_MAT_GOLD �����å�
				glPushMatrix() ;
				{
					glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_SECOND_LIST) ;
				}
				glPopMatrix() ;
			}
			else	// �ÿ˥⡼�����֥顼�ʤ�
			{
//				glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
				glColor3f(currentCL[0], currentCL[1], currentCL[2]) ;	// MATERIAL_MAT_GOLD �����å�
				glPushMatrix() ;
				{
					glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_SECOND_LIST) ;
				}
				glPopMatrix() ;
			}

			// ɽ�̥��饹����
			if (!(detail == 1 || detail == 2) && clock_glass)
			{
				glFogfv(GL_FOG_COLOR, black) ;
				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

				glDepthMask(GL_FALSE) ;
				glEnable(GL_BLEND) ;
//				glBlendFunc(GL_ONE, GL_ONE) ;
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

//				glColor3f(currentCL[0]*0.7f, currentCL[1]*0.7f, currentCL[2]*0.7f) ;
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;

				glDisable(GL_BLEND) ;
				glDepthMask(GL_TRUE) ;

				glFogfv(GL_FOG_COLOR, currentBG) ;
			}

		}
		glPopMatrix() ;
	}
	else	// -REFLECT ���ץ��������
	{
		glDepthMask(GL_FALSE) ;

		// �ƥ������㣱 OFF
		// ���ѥ��ǥ��ƥ�����������ϡ��ƥ�����������
		if (detail < 3)
//			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;

		glActiveTexture(GL_TEXTURE0) ;
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;

		// �ǥץ��񤭹���̵���ǥܡ�������
		if (detail < 3)
		{
			glCallList(BOARD_LIST) ;

			// �ƥ������㣱 ON
			glActiveTexture(GL_TEXTURE1) ;
			glEnable(GL_TEXTURE_2D) ;
		}
		else
		{
			// detail == 3�ʥǥ��ƥ�����ƥ�������ͭ���ǣ��ѥ����ƥ������㤬��ǽ�ʾ���
			// �ܡ���������ϡ��ƥ��������˥åȣ��˥ǥ��ƥ�����ƥ�������򥻥å�

			// �ƥ������㣲��̵����
			glActiveTexture(GL_TEXTURE2) ;
			glDisable(GL_TEXTURE_2D) ;

			// �ƥ������㣱�˥ǥ��ƥ����륻�å�
			glActiveTexture(GL_TEXTURE1) ;
//			glDisable(GL_TEXTURE_GEN_S) ;
//			glDisable(GL_TEXTURE_GEN_T) ;

			glCallList(DETAIL_TEXTURE_LIST) ;
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;

			glCallList(BOARD_LIST) ;

			// �ƥ��������˥åȣ��򥹥ڥ���顼�⡼�ɤ��᤹
			glCallList(SPECULAR_MULTI_TEXTURE_LIST) ;
//			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, currentSP) ;
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND) ;

//			glEnable(GL_TEXTURE_GEN_S) ;
//			glEnable(GL_TEXTURE_GEN_T) ;
		}

		glDepthMask(GL_TRUE) ;

		// ��ɸȿž
		glPushMatrix() ;
		{
			// �Ǥꤳ���������
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			glEnable(GL_BLEND) ;
			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			// �Ǥꤳ�ߥǥ��ƥ���������
			glPushMatrix() ;
			{
				glActiveTexture(GL_TEXTURE0) ;
				glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
				glCallList(MATERIAL_MIRROR_RED_GRAY) ;
				glCallList(ENV_BOARD_DETAIL_LIST) ;		// �ޥƥꥢ��ϥ��åȺѤߤΤ��� ENV_
			}
			glPopMatrix() ;


			// �饤�ƥ��� OFF
			glDisable(GL_LIGHTING) ;

			// �ƥ������㣱 OFF
			glActiveTexture(GL_TEXTURE1) ;
			glDisable(GL_TEXTURE_2D) ;

			// �ƥ������㣰�϶�°����⡼�ɤ�
			glActiveTexture(GL_TEXTURE0) ;
			glCallList(CHROME_TEXTURE_LIST) ;

			// ��ž���֥������ȡ������ζ�°���֥������ȱǤꤳ������

			glPushMatrix() ;
			{
				float refBright = ReflectBrightByCos(zAxis) ;

				// �����Ĥε�����
				glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư

				// OpenGL-1.1
				DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate, refBright, ReflectAlphaByCos(zAxis, 0.4f)) ;

				glShadeModel(GL_SMOOTH) ;

				glPushMatrix() ;
				{
					glScalef(1.0f, 1.0f, -1.0f) ;
					glFrontFace(GL_CCW) ;

					// ���׿�����
//					glDepthMask(GL_TRUE) ;

//					glColor4f(currentCL[0]*COLOR_CHROME_HOUR_R * refBright, currentCL[1]*COLOR_CHROME_HOUR_G * refBright, currentCL[2]*COLOR_CHROME_HOUR_B * refBright, ReflectAlphaByCos(zAxis, 0.5f)) ;	// MATERIAL_MAT_METAL_BLUE �����å�
					glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
					glPushMatrix() ;
						glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_MIRROR_HOUR_LIST) ;
					glPopMatrix() ;

//					glColor4f(currentCL[0]*COLOR_CHROME_MINUTE_R * refBright, currentCL[1]*COLOR_CHROME_MINUTE_G * refBright, currentCL[2]*COLOR_CHROME_MINUTE_B * refBright, ReflectAlphaByCos(zAxis, 0.36f)) ;	// MATERIAL_MAT_METAL_GREEN �����å�
					glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
					glPushMatrix() ;
						glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
						glCallList(ENV_MIRROR_MINUTE_LIST) ;
					glPopMatrix() ;


					glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

					if (s_motion && !motion)
					{
						// �ÿˤ����⡼�����֥顼�ν���
						GLfloat sec_angle = st.start_angle ;
						GLfloat refAlpha = ReflectAlphaByCos(zAxis, 0.3f) ;

						glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
						{
//							glEnable(GL_BLEND) ;
							glDepthMask(GL_FALSE) ;
//							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

							// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//							glColor4f(currentCL[0]*refBright, currentCL[1]*0.75f*refBright, currentCL[2]*0.05f*refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;
							glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, refAlpha / s_motion * S_MOTION_ALPHA) ;

							for (int i = 0 ; i < s_motion ; i ++, sec_angle += st.step_angle)
							{
								if (i == s_motion - 1) glDepthMask(GL_TRUE) ;

								glPushMatrix() ;
									glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
									glCallList(ENV_MIRROR_SECOND_LIST) ;
								glPopMatrix() ;
							}
						}
						glPopAttrib() ;
					}
					else
					{
//						glColor4f(currentCL[0]*refBright, currentCL[1]*0.75f*refBright, currentCL[2]*0.05f*refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�
						glColor4f(currentCL[0]*refBright, currentCL[1]*refBright, currentCL[2]*refBright, ReflectAlphaByCos(zAxis, 0.3f)) ;	// MATERIAL_MAT_GOLD �����å�
						glPushMatrix() ;
							glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_MIRROR_SECOND_LIST) ;
						glPopMatrix() ;
					}

//					glDepthMask(GL_FALSE) ;
					glFrontFace(GL_CW) ;
				}
				glPopMatrix() ;

			}
			glPopMatrix() ;


			// ���αǤ��������
//			glBlendFunc(GL_ONE, GL_ONE) ;
//			glActiveTexture(GL_TEXTURE0) ;
//			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
//			glColor3f(currentCL[0]*0.35f, currentCL[1]*0.35f, currentCL[2]*0.35f) ;
//			glFogfv(GL_FOG_COLOR, black) ;

			// 99-09-23
			glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
			glColor4f(currentCL[0], currentCL[1], currentCL[2], ReflectAlphaByCos(zAxis, 0.16f, 3.0f)) ;
//			glColor4f(currentCL[0], currentCL[1], currentCL[2], Pow(ReflectAlphaByCos(zAxis, 0.4f))) ;


#ifdef CULL_REF_SKY_BY_ANGLE

			if (zAxis > CULL_REF_SKY_AXIS)
			{
				glPushMatrix() ;
				{
					glScalef(1.0f, 1.0f, -1.0f) ;
					glTranslatef(0.0f, 0.0f, -4.2f) ;
					glFrontFace(GL_CCW) ;
					glCallList(ENV_BOARD_LIST) ;
					glFrontFace(GL_CW) ;
				}
				glPopMatrix() ;
			}

#else	// #ifdef CULL_REF_SKY_BY_ANGLE

			glPushMatrix() ;
			{
				glScalef(1.0f, 1.0f, -1.0f) ;
				glTranslatef(0.0f, 0.0f, -3.8f) ;
				glFrontFace(GL_CCW) ;
				glCallList(ENV_BOARD_LIST) ;
				glFrontFace(GL_CW) ;
			}
			glPopMatrix() ;

#endif	// #ifdef CULL_REF_SKY_BY_ANGLE ... #else


//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
//			glFogfv(GL_FOG_COLOR, currentBG) ;

			// �Ǥꤳ��������
			// �饤�ƥ��� ON
			glEnable(GL_LIGHTING) ;

			// �ƥ������㣰����åɥ⡼��
//			glActiveTexture(GL_TEXTURE0) ;
			glCallList(SOLID_TEXTURE_LIST) ;
//			glEnable(GL_TEXTURE_2D) ;

			// �ƥ������㣱���ڥ���顼�⡼��
			glActiveTexture(GL_TEXTURE1) ;
			glEnable(GL_TEXTURE_2D) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;

			glCallList(MIRROR_WALL_LIST) ;

			// �Ǥ���߽�λ
			glFrontFace(GL_CCW) ;
			glDisable(GL_BLEND) ;
		}
		glPopMatrix() ;


		// ���Υǥ��ƥ�����
		glPushMatrix() ;
		{
			glCallList(BOARD_DETAIL_LIST) ;
		}
		glPopMatrix() ;


		// ���ζ�°��ʬ����
		// �饤�ƥ��� OFF
		glDisable(GL_LIGHTING) ;

		// �ƥ������㣱 OFF
		glActiveTexture(GL_TEXTURE1) ;
		glDisable(GL_TEXTURE_2D) ;

		// �ƥ������㣰�϶�°����⡼�ɤ�
		glActiveTexture(GL_TEXTURE0) ;
//		glCallList(CHROME_TEXTURE_LIST) ;
		glCallList(GOLD_TEXTURE_LIST) ;

		// ���������
		// MATERIAL_GOLD �����å�
//		glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;
		glColor3fv(currentCL) ;
		glCallList(ENV_GOLDEN_OBJECTS_LIST) ;

//		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

		// �����׾奪�֥�����������
		glPushMatrix() ;
		{
			// �����Ĥ�¿��������
			glTranslatef(0.0f, 0.0f, 1.2f) ;	// ���Τ�����˰�ư
			// OpenGL-1.1
			DrawMirrorHedronsWithBindingTexture(radius_ratio * HDRN_RADIUS, st.h_rotate) ;
			glShadeModel(GL_SMOOTH) ;

			// ���׿�����

			// ��
//			glColor3f(currentCL[0]*COLOR_CHROME_HOUR_R, currentCL[1]*COLOR_CHROME_HOUR_G, currentCL[2]*COLOR_CHROME_HOUR_B) ;	// MATERIAL_MAT_METAL_BLUE �����å�
			glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
			glPushMatrix() ;
			{
				glRotatef(st.tm.hour, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_HOUR_LIST) ;
			}
			glPopMatrix() ;

			// ʬ
//			glColor3f(currentCL[0]*COLOR_CHROME_MINUTE_R, currentCL[1]*COLOR_CHROME_MINUTE_G, currentCL[2]*COLOR_CHROME_MINUTE_B) ;	// MATERIAL_MAT_METAL_GREEN �����å�
			glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
			glPushMatrix() ;
			{
				glRotatef(st.tm.minute, 0.0f,0.0f,-1.0f) ;
				glCallList(ENV_MINUTE_LIST) ;
			}
			glPopMatrix() ;


			// ��
			glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;	// �⿧�ƥ���������ڤ��ؤ�

			if (s_motion && !motion)
			{
				// �ÿˤ����⡼�����֥顼�ν���
				GLfloat sec_angle = st.start_angle ;//, center_angle ;

				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_BLEND) ;
				{
					glEnable(GL_BLEND) ;
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
					glDepthMask(GL_FALSE) ;

					// ����ե��ͤ��θ���� MATERIAL_MAT_GOLD �����å�
//					glColor4f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f, 1.0f / s_motion * S_MOTION_ALPHA) ;
					glColor4f(currentCL[0], currentCL[1], currentCL[2], 1.0f / s_motion * S_MOTION_ALPHA) ;

					for (int i = 0 ; i < s_motion - 1 ; i ++, sec_angle += st.step_angle)
					{
						// ����ΰ��֤򵭲����Ƥ������Ǹ�����̤����褹���ȾƩ�����򤱤뤿���
//						if (i == (s_motion + 1) / 2)
//						{
//							center_angle = sec_angle ;
//							continue ;
//						}
						glPushMatrix() ;
						{
							glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
							glCallList(ENV_SECOND_LIST) ;
						}
						glPopMatrix() ;
					}
				}
				glPopAttrib() ;
//#ifdef WIN32 // FX_GL_BETA_2
//				glBlendFunc(GL_ONE, GL_ONE) ;
//				glDisable(GL_BLEND) ;
//#endif

//				glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
				glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�
				glPushMatrix() ;
				{
					glRotatef(sec_angle, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_SECOND_LIST) ;
				}
				glPopMatrix() ;
			}
			else	// �ÿ˥⡼�����֥顼�ʤ�
			{
//				glColor3f(currentCL[0], currentCL[1]*0.75f, currentCL[2]*0.05f) ;	// MATERIAL_MAT_GOLD �����å�
				glColor3fv(currentCL) ;	// MATERIAL_MAT_GOLD �����å�
				glPushMatrix() ;
				{
					glRotatef(st.tm.second, 0.0f,0.0f,-1.0f) ;
					glCallList(ENV_SECOND_LIST) ;
				}
				glPopMatrix() ;
			}

			// ɽ�̥��饹����
			if (!(detail == 1 || detail == 2) && clock_glass)
//			if (clock_glass)
			{
				glFogfv(GL_FOG_COLOR, black) ;
				glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

				glDepthMask(GL_FALSE) ;
				glEnable(GL_BLEND) ;
//				glBlendFunc(GL_ONE, GL_ONE) ;
				glBlendFunc(addSrcBlendFactorGlass, addDstBlendFactorGlass) ;

//				glColor3f(currentCL[0]*0.7f, currentCL[1]*0.7f, currentCL[2]*0.7f) ;
				glColor3f(currentCL[0]*envGlassBright, currentCL[1]*envGlassBright, currentCL[2]*envGlassBright) ;
				glCallList(ENV_GLASS_LIST) ;

				glDisable(GL_BLEND) ;
				glDepthMask(GL_TRUE) ;

				glFogfv(GL_FOG_COLOR, currentBG) ;
			}

		}
		glPopMatrix() ;
	}

	glActiveTexture(GL_TEXTURE0) ;
	glDisable(GL_TEXTURE_2D) ;
}


*/


void DrawDetailShell_r2_detail1(ClockState& st)
{
	// �ǥ��ƥ�����ƥ����������	detail == 1
	// �ǥ��ƥ�����ƥ������㥻�å�
	glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
//	glCallList(DETAIL_TEXTURE_LIST) ;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE) ;
	if (fogFlag)
		glDisable(GL_FOG) ;
	glEnable(GL_BLEND) ;
//	glDisable(GL_LIGHTING) ;
	glBlendFunc(multiSrcBlendFactor, multiDstBlendFactor) ;	// �軻�֥��ɥ⡼��
	glDepthMask(GL_FALSE) ;

	glDepthFunc(GL_LEQUAL) ;

	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_DETAILTEX_LIST) ;
		}
		glPopMatrix() ;

		glEnable(GL_CULL_FACE) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_DETAILTEX_LIST_CULL) ;
	else
		glCallList(SHELL_DETAILTEX_LIST) ;

	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (!(cull_status & CULL_STATUS_FRONT))
	{
		// ɽ�����ξ��ܡ�������
		glCallList(BOARD_DETAILTEX_LIST) ;
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;

	glDepthMask(GL_TRUE) ;
	glDisable(GL_BLEND) ;
	glEnable(GL_CULL_FACE) ;
	glEnable(GL_LIGHTING) ;
	if (fogFlag)
		glEnable(GL_FOG) ;
}



// ���������-real 2 ���ץ����ΰ���ܤ������
// Texture Object ����
void DrawSolidClock_texture_object(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
		glEnable(GL_CULL_FACE) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		if (detail == 1)
			DrawDetailShell_r2_detail1(st) ;
		return ;
	}


	if (!reflection)	// ȿ�ͤʤ���
	{
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;
		glCallList(BOARD_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;

		if (detail == 1)
			DrawDetailShell_r2_detail1(st) ;
	}
	else	// -REFLECT ���ץ��������
	{
		glDepthMask(GL_FALSE) ;

		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;
			glCallList(BOARD_LIST) ;	// �ǥץ��񤭹���̵���ǥ���åɤΥܡ�������
		glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;

		if (detail == 1)
			DrawDetailShell_r2_detail1(st) ;

		glDepthMask(GL_TRUE) ;

		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		else
		// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;

		glEnable(GL_BLEND) ;

		glPushMatrix() ;

			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;

				glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
				glCallList(MATERIAL_MIRROR_RED_GRAY) ;
//				glFogfv(GL_FOG_COLOR, white) ;	// �ʤ����ե������顼�������ˤʤ롪��
				glCallList(ENV_BOARD_DETAIL_LIST) ;		// �ޥƥꥢ��ϥ��åȺѤߤΤ��� ENV_
//				glFogfv(GL_FOG_COLOR, currentBG) ;
//				glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;

			glPopMatrix() ;

			glCallList(MIRROR_WALL_LIST) ;

			glFrontFace(GL_CCW) ;
		glPopMatrix() ;

//		glPopAttrib() ;	// s, t ��ɸ�Ϥ��Ȥ��᤹

//		glBlendFunc(GL_ONE, GL_ONE) ;
		glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;

		glDisable(GL_BLEND) ;
		// �Ǥ������ʬ���轪λ

		// ��������
		glPushMatrix() ;
			glCallList(BOARD_DETAIL_LIST) ;
		glPopMatrix() ;
	}
}


// ���������-real 2 ���ץ����ΰ���ܤ������
void DrawSolidClock(ClockState& st)
{
	if (!lid)
	{
		glDisable(GL_CULL_FACE) ;
		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;

		glPushMatrix() ;
		{
			glTranslatef(0.0f, 14.5f, 5.2f) ;
			glRotatef(st.roof * ROOF_RAD, -1.0f,0.0f,0.0f) ;
			glCallList(ROOF_LIST) ;
		}
		glPopMatrix() ;

		if (!lightTwoSide)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;
		glEnable(GL_CULL_FACE) ;
	}

	if (cull_status & CULL_STATUS_SHELL)
		glCallList(SHELL_LIST_CULL) ;
	else
		glCallList(SHELL_LIST) ;


	// -NOCULLOBJ �����ꤵ��Ƥ��ʤ����ϻ��פθ����ˤ�ä����襪�֥������Ȥ� ON/OFF
	if (cull_status & CULL_STATUS_FRONT)
	{
		return ;
	}

	if (!reflection)	// ȿ�ͤʤ���
	{
		glMatrixMode(GL_TEXTURE) ;
		glPushMatrix() ;
			glCallList(SOLID2_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

			glCallList(BOARD_LIST) ;

			glMatrixMode(GL_TEXTURE) ;
		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
	}
	else	// -REFLECT ���ץ��������
	{
		glDepthMask(GL_FALSE) ;

		glMatrixMode(GL_TEXTURE) ;
		glPushMatrix() ;
			glCallList(SOLID2_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

			glCallList(BOARD_LIST) ;	// �ǥץ��񤭹���̵���ǥ���åɤΥܡ�������

			glMatrixMode(GL_TEXTURE) ;
		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;

		glDepthMask(GL_TRUE) ;

		// �Ǥ������ʬ����
		if (envOpenGL != VOODOO_MESA_OLD)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		else
		// 3Dfx Mesa �Ǥϡ��ƥ���������˾�Υե������ϻ��ѤǤ��ʤ�
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR) ;

		glEnable(GL_BLEND) ;

		glPushMatrix() ;

			glScalef(1.0f, 1.0f, -1.0f) ;
			glFrontFace(GL_CW) ;

			glPushMatrix() ;

				glMatrixMode(GL_TEXTURE) ;
				glPushMatrix() ;
					glCallList(SOLID_TEXTURE_LIST) ;
					glMatrixMode(GL_MODELVIEW) ;

						glCallList(MATERIAL_MIRROR_RED_GRAY) ;
//				glFogfv(GL_FOG_COLOR, white) ;	// �ʤ����ե������顼�������ˤʤ롪��
						glCallList(ENV_BOARD_DETAIL_LIST) ;		// �ޥƥꥢ��ϥ��åȺѤߤΤ��� ENV_
//				glFogfv(GL_FOG_COLOR, currentBG) ;

					glMatrixMode(GL_TEXTURE) ;
				glPopMatrix() ;
				glMatrixMode(GL_MODELVIEW) ;

			glPopMatrix() ;

			glCallList(MIRROR_WALL_LIST) ;

			glFrontFace(GL_CCW) ;
		glPopMatrix() ;

//		glPopAttrib() ;	// s, t ��ɸ�Ϥ��Ȥ��᤹

//		glBlendFunc(GL_ONE, GL_ONE) ;
		glBlendFunc(addSrcBlendFactor, addDstBlendFactor) ;

		glDisable(GL_BLEND) ;
		// �Ǥ������ʬ���轪λ

		// ��������
		glPushMatrix() ;
			glCallList(BOARD_DETAIL_LIST) ;
		glPopMatrix() ;
	}
}


void DrawClock_env2(ClockState& st)
{
	GLint tmp_glass ;

	tmp_glass = clock_glass ;
	clock_glass = 0 ;

	if (has_texture_object)
		DrawModulateClock_texture_object(st) ;
	else
		DrawModulateClock(st) ;

	clock_glass = tmp_glass ;

	if (has_texture_object)
	{
		// Texture Object ����
//		glCallList(ENV_GLASS_TEXTURE_LIST) ;
		glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;

		glEnable(GL_BLEND) ;
		glDepthMask(GL_FALSE) ;
		glDepthFunc(GL_LEQUAL) ;

		glFogfv(GL_FOG_COLOR, black) ;

//#ifdef GL_VERSION_1_1
		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif

		DrawModulateClock_texture_object(st) ;

//#ifdef GL_VERSION_1_1
		// OpenGL-1.1 ��
		if (polygon_offset && envOpenGL != VOODOO_3DFX_ICD) glDisable(GL_POLYGON_OFFSET_FILL) ;
//#endif

		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

		glFogfv(GL_FOG_COLOR, currentBG) ;

		glDepthFunc(GL_LESS) ;
		glDisable(GL_BLEND) ;
		glDepthMask(GL_TRUE) ;
	}
	else
	{
		// texture object �ʤ�
		glMatrixMode(GL_TEXTURE) ;
		glPushMatrix() ;
		{
			glCallList(ENV_GLASS_TEXTURE_LIST) ;
			glMatrixMode(GL_MODELVIEW) ;

			glEnable(GL_BLEND) ;
			glDepthMask(GL_FALSE) ;
			glDepthFunc(GL_LEQUAL) ;

			glFogfv(GL_FOG_COLOR, black) ;
			DrawModulateClock(st) ;
			glFogfv(GL_FOG_COLOR, currentBG) ;

			glDepthFunc(GL_LESS) ;
			glDisable(GL_BLEND) ;
			glDepthMask(GL_TRUE) ;

			glMatrixMode(GL_TEXTURE) ;
		}
	
		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
	}
}


void DrawClock_crystal(ClockState& st)
{
	glEnable(GL_BLEND) ;

	glDepthMask(GL_FALSE) ;
	glDisable(GL_DEPTH_TEST) ;

	glDisable(GL_LIGHTING) ;
	glEnable(GL_TEXTURE_2D) ;

	glFogfv(GL_FOG_COLOR, black) ;

	if (field || ((motion || anti) && !(motion && anti)))
	{
		if (anti)
			DrawCrystalClock(st, 1.0f / anti) ;
		else if (field)
			DrawCrystalClock(st, 1.0f / field) ;
		else
			DrawCrystalClock(st, 1.0f / motion) ;
	}
	else
		DrawCrystalClock(st) ;

	glFogfv(GL_FOG_COLOR, currentBG) ;

//	glPopAttrib() ;

}


void DrawClock_env3(ClockState& st)
{
	glDisable(GL_LIGHTING) ;
	glEnable(GL_TEXTURE_2D) ;

	int t_glass = clock_glass ;
	clock_glass = 0 ;
	DrawMirrorClock2(st) ;
	clock_glass = t_glass ;

	glEnable(GL_LIGHTING) ;
	glDisable(GL_TEXTURE_2D) ;
	glDepthFunc(GL_LEQUAL) ;

	glFogfv(GL_FOG_COLOR, black) ;
	DrawShadeWrapClock(st) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	glDepthFunc(GL_LESS) ;
}


void DrawClock_solid3(ClockState& st)
{
#ifdef ________SOLID_SHADED_CLOCK
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	glEnable(GL_LIGHTING) ;
	DrawSolidShadedClock(st) ;

	// Separate Specular Color ���ϡ��ʲ��ν����Ϥʤ�
//	if (mode_solid < 10)	// != 13
	{
		glDepthFunc(GL_LEQUAL) ;

		glFogfv(GL_FOG_COLOR, black) ;
		DrawSpecularClock(st) ;
		glFogfv(GL_FOG_COLOR, currentBG) ;

		glDepthFunc(GL_LESS) ;
	}

#else // #ifdef ________SOLID_SHADED_CLOCK

	glEnable(GL_TEXTURE_2D) ;
	glEnable(GL_LIGHTING) ;
	DrawStandardClock(st) ;
	glDisable(GL_TEXTURE_2D) ;

	// Separate Specular Color ���ϡ��ʲ��ν����Ϥʤ�
	if (mode_solid < 10)	// != 13
	{
		glDepthFunc(GL_LEQUAL) ;

		glFogfv(GL_FOG_COLOR, black) ;
		DrawSpecularClock(st) ;
		glFogfv(GL_FOG_COLOR, currentBG) ;

		glDepthFunc(GL_LESS) ;
	}

#endif	// #ifdef ________SOLID_SHADED_CLOCK
}


void DrawClock_real1(ClockState& st)
{
	// ����åɥޥåԥ󥰥��å�
	glCallList(SOLID_TEXTURE_LIST) ;
	glEnable(GL_TEXTURE_2D) ;

	glDisable(GL_LIGHTING) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	if (has_texture_object)
		DrawSolidModulateClock_texture_object(st) ;
	else
		DrawSolidModulateClock(st) ;

	glDepthFunc(GL_LEQUAL) ;

	glFogfv(GL_FOG_COLOR, black) ;
	DrawEnvSpecularModulateClock(st) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	glDepthFunc(GL_LESS) ;

	// �Ȥꤢ�����ƥ���
	if (detail == 1 || detail == 2 )
	{
		DrawDetailTextureShell(st) ;
	}
}


void DrawClock_real2(ClockState& st)
{
	// ����åɥޥåԥ󥰥��å�
	glCallList(SOLID_TEXTURE_LIST) ;
	glEnable(GL_TEXTURE_2D) ;

	glEnable(GL_LIGHTING) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	if (has_texture_object)
		DrawSolidClock_texture_object(st) ;
	else
		DrawSolidClock(st) ;

	// �Ȥꤢ�����ƥ��ȡʥǥ��ƥ������
/*
#ifdef GL_VERSION_1_1
	if (detail == 1 || detail == 2 )
	{
		DrawDetailModulateShell(st) ;
	}
#endif	// #ifdef GL_VERSION_1_1
*/

	glDisable(GL_LIGHTING) ;
	glDepthFunc(GL_LEQUAL) ;

	glFogfv(GL_FOG_COLOR, black) ;
	DrawEnvSpecularModulateClock(st) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

//	glPopAttrib() ;

	// �Ȥꤢ�����ƥ���
/*
	if (detail == 1 || detail == 2 )
	{
		DrawDetailTextureShell(st) ;
	}
*/
}


//#ifdef GL_VERSION_1_1

void DrawClock_real11(ClockState& st)
{
	return ;
//	glFogfv(GL_FOG_COLOR, currentBG) ;
//	glDisable(GL_LIGHTING) ;
//	DrawMultiTextureModulateClock_multitexture(st) ;
}


void DrawClock_real12(ClockState& st)
{
	glEnable(GL_LIGHTING) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;
	DrawMultiTextureShadingClock_multitexture(st) ;

/*
	if (detail == 1 || detail == 2)
	{
		DrawDetailTextureShell(st) ;
	}
*/
}

//#endif	// #ifdef GL_VERSION_1_1



// ��������ʥ��ơ������ǡ�
void DrawClock(ClockState& st)
{
	if (mode_normal)
	{
		if (light)
			DrawStandardClock(st) ;
		else
		{
			DrawSimpleClock(st) ;

//			for (int i = 0 ; i < 15 ; i ++)
//				DrawSimpleClock(st) ;
		}
	}
	else if (mode_solid)
	{
		switch (mode_solid)
		{
		case 1:
			if (filterTest) // || orgImageFlag)
				DrawSolid1ModulateClockWithFilterTest(st) ;
			else
				DrawSolid1ModulateClock(st) ;
			break ;
		case 2:
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
			DrawSolidShadedClock(st) ;
#else	// #ifdef ________SOLID_SHADED_CLOCK

			DrawStandardClock(st) ;

#endif	// #ifdef ________SOLID_SHADED_CLOCK
			break ;
		case 3:
			DrawClock_solid3(st) ;
			break ;
		case 13:
			DrawSolidShadedClock(st) ;
			break ;
		}
	}
	else if (mode_crystal)
	{
		DrawClock_crystal(st) ;
	}
	else if (mode_env)
	{
		switch (mode_env)
		{
		case 1:
			if (has_texture_object)
				DrawModulateClock_texture_object(st) ;		// �֥��ɤϸ������
			else
				DrawModulateClock(st) ;		// �֥��ɤϸ������
			break ;
		case 2:
//			DrawClock_env2(st) ;
			DrawEnv2Clock_texture_object(st) ;
			break ;
		case 3:
			DrawClock_env3(st) ;
			break ;
		}
	}
	else	// if (mode_real)
	{
		// ���󥹥��å��ΥǥХå��⡼��
		extern GLint DEBUG_modelViewMatrixStack ;
		extern GLint DEBUG_projectionMatrixStack ;
		extern GLint DEBUG_textureMatrixStack ;

		switch (mode_real)
		{
			case 1:
				DrawClock_real1(st) ;
				break ;

			case 2:
			case 3:
				DrawClock_real2(st) ;
				break ;

//#ifdef GL_VERSION_1_1
			case 11:
				DrawClock_real11(st) ;
				break ;

			case 12:
			case 13:
				DrawClock_real12(st) ;
				break ;
//#endif	// #ifdef GL_VERSION_1_1
		}
	}
}


// ��������
// DrawClock() �Υ᥿����ץ����Х���ץ����
// n �ˤϲ����ܤ����褫���Ϥ�
void DrawClock(ClockState& st, int n)
{
	// �����Х���ץ�󥰽���
	// Z�Хåե��Τߥ��ꥢ����
	// Z�Хåե��Τ�����
	// Z�Хåե��ɤ߹��� only
	// ���顼�Хåե��񤭹��ߵ���
	// ADD �� �ط����Τ˶������
	// ADD �� �̾�����

	if (n == 0)
	{
		// �ǽ������
		if (mode_solid == 1)
			DrawSolid1ModulateClockWithOverSampling(st) ;	// �֥��ɤϸ������
		else
		{
			if (has_texture_object)
				DrawModulateClockWithOverSampling_texture_object(st) ;	// �֥��ɤϸ������
			else
				DrawModulateClockWithOverSampling(st) ;	// �֥��ɤϸ������
		}
	}
	else
	{
		float alpha_weight = 1.0f / (n + 1) ;

		// �����ܰʹ�
		glDepthMask(GL_TRUE) ;
		glClear(GL_DEPTH_BUFFER_BIT) ;
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE) ;
		DrawDepthClock(st) ;
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) ;
		glDepthMask(GL_FALSE) ;

		glPushMatrix() ;
		glLoadIdentity() ;
		glTranslatef(0.0f, 0.0f, 0.999999f) ; // far_clip) ;

		glMatrixMode(GL_PROJECTION) ;
		glPushMatrix() ;
		glLoadIdentity() ;

//		glMatrixMode(GL_MODELVIEW) ;
		glDisable(GL_TEXTURE_2D) ;
		if (saver)
			glColor4f(0.0f, 0.0f, 0.0f, alpha_weight) ;
		else
			glColor4f(currentBG[0], currentBG[1], currentBG[2], alpha_weight) ;
		glRectf(-1.0f, -1.0f, 1.0f, 1.0f) ;
		glEnable(GL_TEXTURE_2D) ;

//		glMatrixMode(GL_PROJECTION) ;
		glPopMatrix() ;

		glMatrixMode(GL_MODELVIEW) ;
		glPopMatrix() ;

		if (mode_solid == 1)
			DrawSolid1ModulateClockWithOverSampling(st, alpha_weight, n) ;
		else
		{
			if (has_texture_object)
				DrawModulateClockWithOverSampling_texture_object(st, alpha_weight, n) ;
			else
				DrawModulateClockWithOverSampling(st, alpha_weight, n) ;
		}
	}
}

// ʸ��������
void DrawBoard(GLint divide, GLfloat radius, int normalFlag = TRUE, float *s = NULL, float *t = NULL,
			   int secondTexUnit = -1, float *s2 = NULL, float *t2 = NULL)
{


//	if (secondTexUnit != -1)
//		secondTexUnit = 1 ;


	int	  i ;

	glBegin(GL_TRIANGLE_FAN) ;
	{
		if (normalFlag) glNormal3f(0.0f, 0.0f, 1.0f) ;	// ˡ���٥��ȥ�����Ʊ��
		Vertex3fWithTexAxis(0.0f, 0.0f, 0.0f, 0, s, t, secondTexUnit, s2, t2) ;	// ����ʸ�����濴��ɸ
//		Vertex3fWithTexAxis(0.0f, 0.0f, 0.0f, s, t) ;	// ����ʸ�����濴��ɸ
/*
		if (s && t)
			TexCoord2f(0.0f, 0.0f) ;
//			TexCoord2f(s[0] * 0.0f + s[1] * 0.0f + s[2] * 0.0f, t[0] * 0.0f + t[1] * 0.0f + t[2] * 0.0f) ;
		glVertex3f(0.0f, 0.0f, 0.0f) ;	// ����ʸ�����濴��ɸ
*/
/*
		for (i = 0 ; i < divide ; i ++)
		{
			GLfloat rad = -2.0f * PAI / divide * i ;
			glVertex3f(sin(rad) * radius, cos(rad) * radius, 0.0f) ;
		}
		glVertex3f(0.0f, radius, 0.0f) ;	// �������Ƚ�λ����Ϣ�� 
*/
		for (i = 0 ; i <= divide ; i ++)
		{
			GLfloat rad = -2.0f * PAI / divide * i ;
			if (i == divide) rad = 0.0f ;
			GLfloat x = sin(rad), y = cos(rad) ;
			Vector3d norm(x * 0.15f * norm_ratio, y * 0.15f * norm_ratio, 1.0f) ;

			if (normalFlag) glNormal3fv(norm.Unit().v) ;	// ˡ���٥��ȥ�Ͼ�����������
			Vertex3fWithTexAxis(x * radius, y * radius, 0.0f, 0, s, t, secondTexUnit, s2, t2) ;
//			Vertex3fWithTexAxis(x * radius, y * radius, 0.0f, s, t) ;
/*			if (s && t)
				TexCoord2f(s[0] * x * radius + s[1] * y * radius + s[2] * 0.0f, t[0] * x * radius + t[1] * y * radius + t[2] * 0.0f) ;
			glVertex3f(x * radius, y * radius, 0.0f) ;
*/
		}
	}
	glEnd() ;
}

// ʸ���׾�ξ�����γ������
void DrawClockPoints(GLfloat radius, float *s = NULL, float *t = NULL)
{
	int i ;
	for (i = 0 ; i < 60 ; i ++)
	{
		if ((i % 5) == 0) i ++ ;
		glPushMatrix() ;
			glRotatef(-6.0f * i, 0.0f,0.0f,1.0f) ;
			glTranslatef(0.0f, radius, 0.0f) ;
			DrawClockPoint(1.5f, 2.0f, 3.0f, s, t) ;
//			glCallList(POINT_LIST) ;
		glPopMatrix() ;
	}
}

// ���׿�����ʼºݤ������
void DrawNeedlePolygons(Object **needle, float *s = NULL, float *t = NULL)
{
//	for (n = 0 ; n < 4 ; n ++)
	for (int e = 0 ; e < 6 ; e ++)
	{
		static int element[] = { 0,1, 4,5, 2,3 } ;
		int n = element[e] ;
		int p = n % 2 ;

		glBegin(GL_TRIANGLE_FAN) ;
		{
			if (flag_glNormal) glNormal3fv(needle[n]->polygon[1].normal[2].v) ;
			if (s && t)
			{
				Vector3d& v = needle[n]->polygon[0].vertex[2] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(needle[n]->polygon[0].vertex[2].v) ;

			for (int i = p*6 ; i != 8*(1-p)-1 ; i += 1-p*2)
			{
				if (flag_glNormal) glNormal3fv(needle[n]->polygon[i].normal[p].v) ;
				if (s && t)
				{
					Vector3d& v = needle[n]->polygon[i].vertex[p] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(needle[n]->polygon[i].vertex[p].v) ;
			}
		}
		glEnd() ;


		glBegin(GL_TRIANGLES) ;

			for (int i = 0 ; i < 3 ; i ++)
			{
				if (flag_glNormal) glNormal3fv(needle[n]->polygon[6].normal[i].v) ;
				if (s && t)
				{
					Vector3d& v = needle[n]->polygon[6].vertex[i] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(needle[n]->polygon[6].vertex[i].v) ;
			}

		glEnd() ;

	}
}


// ���׿�����ʥǡ���������
void DrawNeedle(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, float *s = NULL, float *t = NULL)
{
	int            i, n ;
	Object **needle ;
	GLfloat       m[] = {     x,   0.0f, 0.0f, 0.0f,
					       0.0f, y*1.4f, 0.0f, 0.0f,
					       0.0f,   0.0f,    z, 0.0f,
						   0.0f,   0.0f, 0.0f, 1.0f } ;

	needle = new Object*[6] ;

//	for (n = 0 ; n < 4 ; n ++)
	for (n = 0 ; n < 6 ; n ++)
	{
		int p = n % 2 ;

		if      (n == 1) m[0] = -x ;
		else if (n == 2) m[5] = -y * 0.1f ;
		else if (n == 3) m[0] =  x ;
		else if (n == 4)
		{
			m[0]  = -x * 0.7f ;
			m[5]  = -y * 0.8f ;
			m[10] =  z * 0.7f ;

			m[13] =  y * (-0.15f) ;	// ������ư�ʤ���ǤԤä�����ס�
		}
		else if (n == 5) m[0] = x * 0.7f ;

		needle[n] = new Object(7) ;


		for (i = 0 ; i < 7 ; i ++)
			needle[n]->MakePolygon(i, i+p,i+1-p,8, n_v, m) ;

		// �������֥ݥꥴ��Ρ��裰�裱ĺ�������ܥݥꥴ���ˡ����ʿ��
		for (i = 1 ; i < 5 ; i ++)
		{
			needle[n]->polygon[i].normal[2] = needle[n]->polygon[0].pNormal ;
			needle[n]->AverageNormal(i,   p, i,i-1) ;
			needle[n]->AverageNormal(i, 1-p, i,i+1) ;
		}

		needle[n]->polygon[5].normal[2] = needle[n]->polygon[0].pNormal ;
		needle[n]->AverageNormal(0, 1-p, 0,1) ;
		needle[n]->AverageNormal(5,   p, 4,5) ;
	}

	DrawNeedlePolygons(needle, s, t) ;

	for (n = 0 ; n < 6 ; n ++) delete needle[n] ;
	delete needle ;
}

// ������ꥹ�Ⱥ���
void MakeSignList()
{
	glNewList(AllocAList(SIGN_LIST), GL_COMPILE) ;
		DrawString(sign, GLUT_BITMAP_TIMES_ROMAN_10) ;
	glEndList() ;
}

// �����ꥹ�Ⱥ���
void MakeGoldenObjectList()
{
	glNewList(AllocAList(GOLDEN_OBJECT_LIST), GL_COMPILE) ;
		DrawGoldenObject(1.1f,1.1f,1.0f, gold_depth, TRUE, texture, flag_glNormal) ;
	glEndList() ;
}

// ʸ���׾�ξ�����γ�ꥹ�Ⱥ���
void MakeClockPointList()
{
	if (clock_point)
	{
		glNewList(AllocAList(POINT_LIST), GL_COMPILE) ;
			DrawClockPoint(1.5f, 2.0f, 3.0f) ;
		glEndList() ;
	}
}

// �ͥꥹ�Ⱥ���
void MakeMList()
{
	glNewList(AllocAList(M_LIST), GL_COMPILE) ;
		DrawM() ;
	glEndList() ;
}

// ���׿˥ꥹ�Ⱥ���
void MakeClockNeedleLists()
{
	// ������
	// -s 2,3, -e 3, normal �Τ�
	glNewList(AllocAList(HOUR_LIST), GL_COMPILE) ;
	{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
		if (mode_solid)
			glColor3f(COLOR_HOUR_SOLID_R, COLOR_HOUR_SOLID_G, COLOR_HOUR_SOLID_B) ;	// -solid ����
		else if (mode_env == 3)
//			glCallList(MATERIAL_METAL_BLUE) ;	 // -e 3
			SetMaterial(GL_FRONT, COLOR_HOUR_R, COLOR_HOUR_G, COLOR_HOUR_B, 1.0f, 1.0f, 2.0f * spec, 128.0f) ;
		else if (mode_normal && light)
//			glCallList(MATERIAL_MAT_METAL_BLUE) ;	// normal
			SetMaterial(GL_FRONT, COLOR_HOUR_R, COLOR_HOUR_G, COLOR_HOUR_B, 1.0f, 0.8f, 1.3f * spec, 32.0f) ;
		else	// mode_normal && !light
			glColor3f(COLOR_HOUR_R, COLOR_HOUR_G, COLOR_HOUR_B) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK
	
		if (texture < 3 || solid) glCallList(MATERIAL_MAT_METAL_BLUE) ;
		else                      glCallList(MATERIAL_METAL_BLUE) ;	 // -e 3

#endif	// #ifdef ________SOLID_SHADED_CLOCK

		glTranslatef(0.0f, N_HOUR_TRANS_Y * radius_ratio, N_HOUR_TRANS_Z) ;
		if (flag_shadeTexCoordOnlySolid)
			DrawNeedle(N_HOUR_WIDTH * N_D_WIDTH * n_width, N_HOUR_LENGTH * radius_ratio, N_HOUR_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_HOUR_WIDTH * N_D_WIDTH * n_width, N_HOUR_LENGTH * radius_ratio, N_HOUR_HEIGHT) ;

	}
	glEndList() ;

	glNewList(AllocAList(ENV_HOUR_LIST), GL_COMPILE) ;
	{
		glTranslatef(0.0f, N_HOUR_TRANS_Y * radius_ratio, N_HOUR_TRANS_Z) ;
		if (flag_modulateTexCoord)
			DrawNeedle(N_HOUR_WIDTH * N_D_WIDTH * n_width, N_HOUR_LENGTH * radius_ratio, N_HOUR_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_HOUR_WIDTH * N_D_WIDTH * n_width, N_HOUR_LENGTH * radius_ratio, N_HOUR_HEIGHT) ;
	}
	glEndList() ;


	glNewList(AllocAList(MINUTE_LIST), GL_COMPILE) ;
	{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK

		if (mode_solid)
			glColor3f(COLOR_MINUTE_R, COLOR_MINUTE_G, COLOR_MINUTE_B) ;
		else if (mode_env == 3)
//			glCallList(MATERIAL_METAL_GREEN) ;	 // -e 3
			SetMaterial(GL_FRONT, COLOR_MINUTE_R, COLOR_MINUTE_G, COLOR_MINUTE_B, 1.0f, 1.0f, 2.0f * spec, 128.0f) ;
		else if (mode_normal && light)
//			glCallList(MATERIAL_MAT_METAL_GREEN) ;	// normal
			SetMaterial(GL_FRONT, COLOR_MINUTE_R, COLOR_MINUTE_G, COLOR_MINUTE_B, 1.0f, 0.8f, 1.3f * spec, 32.0f) ;
		else	// mode_normal && !light
			glColor3f(COLOR_MINUTE_R, COLOR_MINUTE_G, COLOR_MINUTE_B) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

		if (texture < 3 || solid) glCallList(MATERIAL_MAT_METAL_GREEN) ;
		else                      glCallList(MATERIAL_METAL_GREEN) ;
#endif	// #ifdef ________SOLID_SHADED_CLOCK

		glTranslatef(0.0f, N_MINUTE_TRANS_Y * radius_ratio, N_MINUTE_TRANS_Z) ;
		if (flag_shadeTexCoordOnlySolid)
			DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT) ;
	}
	glEndList() ;

	glNewList(AllocAList(ENV_MINUTE_LIST), GL_COMPILE) ;
	{
		glTranslatef(0.0f, N_MINUTE_TRANS_Y * radius_ratio, N_MINUTE_TRANS_Z) ;
		if (flag_modulateTexCoord)
			DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT) ;
	}
	glEndList() ;


	glNewList(AllocAList(SECOND_LIST), GL_COMPILE) ;
	{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
		if (mode_solid)
			glColor4f(COLOR_SECOND_R, COLOR_SECOND_G, COLOR_SECOND_B, 1.0f / pre_s_motion * PRE_S_MOTION_A) ;
//			SetMaterial(GL_FRONT, COLOR_SECOND_R, COLOR_SECOND_G, COLOR_SECOND_B, 1.0f / pre_s_motion * PRE_S_MOTION_A, 0.0f, 2.0f * spec * specular) ;
		else if (mode_env == 3)
			glCallList(MATERIAL_GOLD) ;	 // -e 3
		else if (mode_normal && light)
//			glCallList(MATERIAL_MAT_GOLD) ;	// normal
			SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f / pre_s_motion * PRE_S_MOTION_A, 0.8f, 1.3f * specular * spec, 32.0f) ;
		else	// mode_normal && !light
			glColor4f(COLOR_SECOND_R, COLOR_SECOND_G, COLOR_SECOND_B, 1.0f / pre_s_motion * PRE_S_MOTION_A) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

		if (texture < 3 || solid) glCallList(MATERIAL_MAT_GOLD) ;
		else                      glCallList(MATERIAL_GOLD) ;

#endif	// #ifdef ________SOLID_SHADED_CLOCK

		glTranslatef(0.0f, N_SECOND_TRANS_Y * radius_ratio, N_SECOND_TRANS_Z) ;
		if (flag_shadeTexCoordOnlySolid)
			DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT) ;
	}
	glEndList() ;

	// -e 3 && -sm �Ѥ��ÿ˥ꥹ�ȡʵ��٤� 1/pre_s_motion �ˡ�
	glNewList(AllocAList(SECOND_E3_LIST), GL_COMPILE) ;
	{
//		SetMaterial(GL_FRONT, 1.0f/pre_s_motion,0.75f/pre_s_motion,0.05f/pre_s_motion, 1.0f, 1.0f, /*0.95f*/ 2.0f * specular * spec / pre_s_motion, 128.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
		SetMaterial(GL_FRONT, COLOR_SECOND_R / pre_s_motion, COLOR_SECOND_G / pre_s_motion, COLOR_SECOND_B / pre_s_motion, 1.0f, 1.0f, 2.0f * spec / pre_s_motion, 128.0f) ;
		glTranslatef(0.0f, N_SECOND_TRANS_Y * radius_ratio, N_SECOND_TRANS_Z) ;
		DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT) ;
	}
	glEndList() ;

	glNewList(AllocAList(ENV_SECOND_LIST), GL_COMPILE) ;
	{
		glTranslatef(0.0f, N_SECOND_TRANS_Y * radius_ratio, N_SECOND_TRANS_Z) ;
		if (flag_modulateTexCoord)
			DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT, sAxis.v, tAxis.v) ;
		else
			DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT) ;
	}
	glEndList() ;


	if (!reflection) return ;


	// ʸ���פؤαǤ�����ѡ�-real 2 �ʾ������Τߡ�z��ȿ�Ф˰�ư
	glNewList(AllocAList(ENV_MIRROR_HOUR_LIST), GL_COMPILE) ;
		glTranslatef(0.0f, N_HOUR_TRANS_Y * radius_ratio, -N_HOUR_TRANS_Z) ;
		DrawNeedle(N_HOUR_WIDTH * N_D_WIDTH * n_width, N_HOUR_LENGTH * radius_ratio, N_HOUR_HEIGHT * N_MIRROR_HEIGHT_RATIO) ;
	glEndList() ;


	glNewList(AllocAList(MIRROR_MINUTE_LIST), GL_COMPILE) ;
		if (texture < 3 || solid) glCallList(MATERIAL_MAT_METAL_GREEN) ;
		else                      glCallList(MATERIAL_METAL_GREEN) ;
		glTranslatef(0.0f, N_MINUTE_TRANS_Y * radius_ratio, -N_MINUTE_TRANS_Z) ;
		DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT * N_MIRROR_HEIGHT_RATIO) ;
	glEndList() ;

	glNewList(AllocAList(ENV_MIRROR_MINUTE_LIST), GL_COMPILE) ;
		glTranslatef(0.0f, N_MINUTE_TRANS_Y * radius_ratio, -N_MINUTE_TRANS_Z) ;
		DrawNeedle(N_MINUTE_WIDTH * N_D_WIDTH * n_width, N_MINUTE_LENGTH * radius_ratio, N_MINUTE_HEIGHT * N_MIRROR_HEIGHT_RATIO) ;
	glEndList() ;


	glNewList(AllocAList(MIRROR_SECOND_LIST), GL_COMPILE) ;
		if (texture < 3 || solid) glCallList(MATERIAL_MAT_GOLD) ;
		else                      glCallList(MATERIAL_GOLD) ;
		glTranslatef(0.0f, N_SECOND_TRANS_Y * radius_ratio, -N_SECOND_TRANS_Z) ;
		DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT * N_MIRROR_HEIGHT_RATIO) ;
	glEndList() ;

	glNewList(AllocAList(ENV_MIRROR_SECOND_LIST), GL_COMPILE) ;
		glTranslatef(0.0f, N_SECOND_TRANS_Y * radius_ratio, -N_SECOND_TRANS_Z) ;
		DrawNeedle(N_SECOND_WIDTH * N_D_WIDTH * n_width, N_SECOND_LENGTH * radius_ratio, N_SECOND_HEIGHT * N_MIRROR_HEIGHT_RATIO) ;
	glEndList() ;
}


// ���פγ�����
void DrawRoof(int divide, int mesh = 1, int grad_sw = TRUE,
			  float *s = NULL, float *t = NULL, float alpha = 1.0f,
			  int secondTexUnit = -1, float *s2 = NULL, float *t2 = NULL)
{


//	if (secondTexUnit != -1)
//		secondTexUnit = 1 ;


	RollingObject roof(divide * (n_r_vtx[mesh] - 1)) ;

	if (mesh)
	{
		roof.MakeRollingPolygons(divide, n_r_vtx[mesh], r_v) ;
		roof.AverageNormals(TRUE /*clock_div >= 12 */,  TRUE, r_a) ;
	}
	else
	{
		roof.MakeRollingPolygons(divide, n_r_vtx[mesh], r2_v) ;
		roof.AverageNormals(TRUE /* clock_div >= 12 */,  TRUE, r2_a) ;
	}

	roof.TopNormal() ;	// ���־��ĺ����ˡ���Ͽ��������

	glTranslatef(0.0f, -14.5f, -5.2f) ;


	// �Ƥäڤ�λ��ѥݥꥴ������
	int i = divide - 1 ;
	int pNo = i * (n_r_vtx[mesh]  - 1), n ;

	glBegin(GL_TRIANGLE_FAN) ;
	{
		if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[0].v) ;

		if (grad_sw)
		{
			if (alpha == 1.0f)
				glColor3f(0.7f, 0.7f, 0.7f) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
			else
				glColor4f(0.7f, 0.7f, 0.7f, alpha) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
		}
		Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//		Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[0].v, s, t) ;

		n = (i + 1) % divide ;
		if (grad_sw)
		{
			if (alpha == 1.0f)
				glColor3fv(rgrad[n % ngrad].v) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
			else
				glColor4f(rgrad[n % ngrad].v[0], rgrad[n % ngrad].v[1], rgrad[n % ngrad].v[2], alpha) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
		}

		if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[1].v) ;
		Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//		Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[1].v, s, t) ;

		for (i = divide - 1 ; i >= 0 ; i --)
		{
			n = i ;	// n = (i + 1 - 1) % divide ;

			if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[2].v) ;
			if (grad_sw)
			{
				if (alpha == 1.0f)
					glColor3fv(rgrad[n % ngrad].v) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
				else
					glColor4f(rgrad[n % ngrad].v[0], rgrad[n % ngrad].v[1], rgrad[n % ngrad].v[2], alpha) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
			}
			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[2].v, s, t) ;

			pNo -= n_r_vtx[mesh]  - 1 ;
		}
	}
	glEnd() ;


	// �̾�λͳѥݥꥴ������
	int u, v ;
	for (v = 1 ; v < n_r_vtx[mesh] - 1 ; v ++)
	{
		pNo = v ;

		glBegin(GL_QUAD_STRIP) ;
//		glBegin(GL_TRIANGLE_STRIP) ;
		{
			n = 0 ; //	n = (-1 + 1) % divide ;
			if (grad_sw)
			{
				if (alpha == 1.0f)
					glColor3fv(rgrad[n % ngrad].v) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
				else
					glColor4f(rgrad[n % ngrad].v[0], rgrad[n % ngrad].v[1], rgrad[n % ngrad].v[2], alpha) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
			}

			if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[2].v) ;
			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[2].v, s, t) ;

			if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[3].v) ;
			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[3].v, 0, s, t, secondTexUnit, s2, t2) ;
//			Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[3].v, s, t) ;

			for (u = 0 ; u < divide ; u ++)
			{
				n = (u + 1) % divide ;
				if (grad_sw)
				{
					if (alpha == 1.0f)
						glColor3fv(rgrad[n % ngrad].v) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
					else
						glColor4f(rgrad[n % ngrad].v[0], rgrad[n % ngrad].v[1], rgrad[n % ngrad].v[2], alpha) ;	// �ϥ��饤�Ȥο��򥰥�ǡ������
				}

				if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[1].v) ;
				Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//				Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[1].v, s, t) ;

				if (flag_glNormal) glNormal3fv(roof.polygon[pNo].normal[0].v) ;
				Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//				Vertex3fvWithTexAxis(roof.polygon[pNo].vertex[0].v, s, t) ;

				pNo += n_r_vtx[mesh] - 1 ;
			}
		}
		glEnd() ;
	}
}


// ʸ���פرǤ�����Ѥβ���������
// alpha_flag �����åȤ���Ƥ����硢����ե��ͤ� max ���� min �δ֤ǥ���ǡ������
void DrawMirrorWall(int divide, int alpha_flag = TRUE, GLfloat grad_max = 1.0f, GLfloat grad_min = 0.0f, float *s = NULL, float *t = NULL)
{
	int i ;

	RollingObject wall(divide) ;

	wall.MakeRollingPolygons(divide, 2, mw_v) ;
	wall.AverageNormals(U_SMOOTH_FLAG /* divide >= 12 */) ; //, FALSE) ;

	if (!(U_SMOOTH_FLAG))
	{
		glBegin(GL_QUADS) ;

			for (i = 0 ; i < divide ; i ++)
			{
				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_max) ;
				if (flag_glNormal) glNormal3fv(wall.polygon[i].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[0].v) ;

				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_min) ;
				if (flag_glNormal) glNormal3fv(wall.polygon[i].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[1].v) ;

				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_min) ;
				if (flag_glNormal) glNormal3fv(wall.polygon[i].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[2].v) ;

				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_max) ;
				if (flag_glNormal) glNormal3fv(wall.polygon[i].normal[3].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[3] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[3].v) ;
			}

		glEnd() ;
	}
	else
	{
		glBegin(GL_QUAD_STRIP) ;

			if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_min) ;
			if (flag_glNormal) glNormal3fv(wall.polygon[0].normal[2].v) ;
			if (s && t)
			{
				Vector3d& v = wall.polygon[0].vertex[2] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(wall.polygon[0].vertex[2].v) ;

			if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_max) ;
			if (flag_glNormal) glNormal3fv(wall.polygon[0].normal[3].v) ;
			if (s && t)
			{
				Vector3d& v = wall.polygon[0].vertex[3] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(wall.polygon[0].vertex[3].v) ;

			for (i = 0 ; i < divide ; i ++)
			{
				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_min) ;
				if (flag_glNormal) glNormal3fv(wall.polygon[i].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[1].v) ;

				if (alpha_flag) glColor4f(1.0f,1.0f,0.8f, grad_max) ;
//				glNormal3fv(wall.polygon[i].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = wall.polygon[i].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(wall.polygon[i].vertex[0].v) ;
			}

		glEnd() ;
	}
}


// ���פγ�����
// firstWallFlag��-r �Υǥ��ƥ�����ƥ�������ξ�硢�ǽ���ɤ����褷�ʤ���
// detail == 3
// �ĤޤꡢsecondTexUnit �ʹߤ����ꤵ��Ƥ����硢
// �ǽ���ɤΤߥƥ������㣲�� Disable�����θ�ǥƥ������㣲�� Enable �ˤ���
void DrawShell(int divide, int mesh = 1, int reverseFaceCull = FALSE,
			   int normalFlag = TRUE, float *s = NULL, float *t = NULL,
			   int firstWallFlag = TRUE,
			   int secondTexUnit = -1, float *s2 = NULL, float *t2 = NULL)
{
//	if (secondTexUnit != -1)
//		secondTexUnit = 0 ;


#ifdef DRAW_DETAIL_FIRST_WALL
	firstWallFlag = TRUE ;
#endif	// #ifdef DRAW_DETAIL_FIRST_WALL

/*
	if (secondTexUnit != -1 && secondTexUnit != 1)
	{
		logprintf("Error in DrawShell()\n") ;
		logprintf("\tsecondTexUnit = %d\n", secondTexUnit) ;
	}
*/

	int s_cullNo ;
	// reverseFaceCull �����ꤵ��Ƥ����硢�����ֹ�ʹߤ�΢�̤Ȥߤʤ����褷�ʤ�
	if (reverseFaceCull)
	{
		if (mesh)
			s_cullNo = 11 ;
		else
			s_cullNo = 6 ;
	}

	RollingObject shell(divide * (n_s_vtx[mesh] - 1)) ;

	if (mesh)
	{
		shell.MakeRollingPolygons(divide, n_s_vtx[mesh], s_v) ;
		shell.AverageNormals(U_SMOOTH_FLAG, v_smooth, V_SMOOTH_T(s_a)) ;
	}
	else
	{
		shell.MakeRollingPolygons(divide, n_s_vtx[mesh], s2_v) ;
		shell.AverageNormals(U_SMOOTH_FLAG, v_smooth, V_SMOOTH_T(s2_a)) ;
	}

	// u_smooth ��Ԥ�ʤ��ä���硢�����ʬ�򥹥ࡼ���ˤ���
	if (!(U_SMOOTH_FLAG))
	{
		for (int u = 0 ; u < divide ; u ++)
		{
			int      p0No = u * (n_s_vtx[mesh] - 1) + n_s_vtx[mesh] - 2, p1No ;
			Vector3d norm ;

			if (u == divide - 1) p1No = p0No - (n_s_vtx[mesh] - 1) * (divide - 1) ;
			else                 p1No = p0No + (n_s_vtx[mesh] - 1) ;

			norm = Unit(shell.polygon[p0No].normal[0] + shell.polygon[p1No].normal[2]) ;
			shell.polygon[p0No].normal[0] = shell.polygon[p1No].normal[2] = norm ;
		}
	}

	shell.BottomNormal() ;	// ���ֲ���ĺ����ˡ���Ͽ���������

	if (!(U_SMOOTH_FLAG))
	{
		int i ;
		for (i = 0 ; i < divide ; i ++)
		{
			int pNo, j ;

			glBegin(GL_QUADS) ;
			{
#ifndef DRAW_DETAIL_FIRST_WALL
				int texUnit = secondTexUnit ;
#endif	// #ifndef DRAW_DETAIL_FIRST_WALL

				for (j = 0 ; j < n_s_vtx[mesh] - 2 ; j ++)
				{
					// reverseFaceCull �����ꤵ��Ƥ�����ϡ�����ξ������轪λ
					if (reverseFaceCull && j >= s_cullNo)
						break ;

					// �ǥ��ƥ�����ƥ��������Ѥ�����ξ�硢
					// �ǽ���ɤ���̵��
#ifndef DRAW_DETAIL_FIRST_WALL
					if (!firstWallFlag && j == 0)
						continue ;

					// �ǥ��ƥ�����ƥ�������ʣ��ѥ��ޥ���ƥ��������
					// �ǽ���ɤΤ� secondTexUnit ��̵���ˤ���
					if (texUnit != -1)	// firstWallFlag �ˤ�̵�ط�
					{
						if (j == 0)
						{
							glActiveTexture((GLenum)((int)GL_TEXTURE0 + secondTexUnit)) ;
							glDisable(GL_TEXTURE_2D) ;
							secondTexUnit = -1 ;
						}
						else if (j == 1)
						{
							// �ǽ���ɰʳ��ϡ��ƥ��������˥åȣ���ͭ����
							secondTexUnit = texUnit ;
							glEnable(GL_TEXTURE_2D) ;
						}
					}
#endif	// #ifndef DRAW_DETAIL_FIRST_WALL

					pNo = i * (n_s_vtx[mesh] - 1) + j ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[0].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, s, t) ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[1].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, s, t) ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[2].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, s, t) ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[3].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[3].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[3].v, s, t) ;
				}
			}
			glEnd() ;

			pNo ++ ;
			glBegin(GL_TRIANGLES) ;
			{
				if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[0].v) ;
				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, s, t) ;

				if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[1].v) ;
				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, s, t) ;

				if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[2].v) ;
				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//				Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, s, t) ;
			}
			glEnd() ;
		}
	}
	else
	{
		int u, v ;

#ifndef DRAW_DETAIL_FIRST_WALL
		int texUnit = secondTexUnit ;
#endif	// #ifndef DRAW_DETAIL_FIRST_WALL

		for (v = 0 ; v < n_s_vtx[mesh] - 1 ; v ++)
		{
			// reverseFaceCull �����ꤵ��Ƥ�����ϡ�����ξ������轪λ
			if (reverseFaceCull && v >= s_cullNo)
				break ;

			// �ǥ��ƥ�����ƥ��������Ѥ�����ξ�硢
			// �ǽ���ɤ���̵��
//			if (!firstWallFlag && v == 0)
//				continue ;

			// �ǥ��ƥ�����ƥ��������Ѥ�����ξ�硢
			// �ǽ���ɤ���̵��
#ifndef DRAW_DETAIL_FIRST_WALL
			if (!firstWallFlag && j == 0)
				continue ;

			// �ǥ��ƥ�����ƥ�������ʣ��ѥ��ޥ���ƥ��������
			// �ǽ���ɤΤ� secondTexUnit ��̵���ˤ���
			if (texUnit != -1)	// firstWallFlag �ˤ�̵�ط�
			{
				if (j == 0)
				{
					glActiveTexture((GLenum)((int)GL_TEXTURE0 + secondTexUnit)) ;
					glDisable(GL_TEXTURE_2D) ;
					secondTexUnit = -1 ;
				}
				else if (j == 1)
				{
					// �ǽ���ɰʳ��ϡ��ƥ��������˥åȣ���ͭ����
					secondTexUnit = texUnit ;
					glEnable(GL_TEXTURE_2D) ;
				}
			}
#endif	// #ifndef DRAW_DETAIL_FIRST_WALL

			if (v == n_s_vtx[mesh] - 2 ) // ��λ��ѷ�
			{
				int pNo = v ;

				glBegin(GL_TRIANGLE_FAN) ;
				{
					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[1].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, s, t) ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[2].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, s, t) ;

					for (u = 0 ; u < divide ; u ++)
					{
						if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[0].v) ;
						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, s, t) ;

						pNo += n_s_vtx[mesh] - 1 ;
					}
				}
				glEnd() ;
			}
			else  // �̾�λͳѷ�
			{
				int pNo = v ;

				glBegin(GL_QUAD_STRIP) ;
				{
					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[2].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[2].v, s, t) ;

					if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[3].v) ;
					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[3].v, 0, s, t, secondTexUnit, s2, t2) ;
//					Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[3].v, s, t) ;

					for (u = 0 ; u < divide ; u ++)
					{
						if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[1].v) ;
						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, 0, s, t, secondTexUnit, s2, t2) ;
//						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[1].v, s, t) ;

						if (normalFlag) glNormal3fv(shell.polygon[pNo].normal[0].v) ;
						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, 0, s, t, secondTexUnit, s2, t2) ;
//						Vertex3fvWithTexAxis(shell.polygon[pNo].vertex[0].v, s, t) ;

						pNo += n_s_vtx[mesh] - 1 ;
					}
				}
				glEnd() ;
			}
		}
	}
}

// ʸ���׾�Σ����Ĥ�γ
void MakePolyhedronLists()
{
	// ��¿���Υꥹ�Ⱥ���
	glNewList(AllocAList(HEDRON_LIST), GL_COMPILE) ;
		if (flag_shadeTexCoordOnlySolid || flag_modulateTexCoord)
			DrawPolyhedron(0.7f, 0.7f, 0.7f, sAxis.v, tAxis.v) ;
		else
			DrawPolyhedron(0.7f, 0.7f, 0.7f) ;
	glEndList() ;

	// ��¿���Υꥹ�ȣ������ʣ��������ѡ�
	glNewList(AllocAList(HEDRON_LIST2), GL_COMPILE) ;
		if (flag_shadeTexCoordOnlySolid || flag_modulateTexCoord)
			DrawPolyhedron(1.2f, 1.2f, 1.2f, sAxis.v, tAxis.v) ;
		else
			DrawPolyhedron(1.2f, 1.2f, 1.2f) ;
	glEndList() ;

	// ��¿���Υꥹ�ȣ������ʣ������ѡ�
	glNewList(AllocAList(HEDRON_LIST3), GL_COMPILE) ;
		if (flag_shadeTexCoordOnlySolid || flag_modulateTexCoord)
			DrawPolyhedron(0.95f, 0.95f, 0.95f, sAxis.v, tAxis.v) ;
		else
			DrawPolyhedron(0.95f, 0.95f, 0.95f) ;
	glEndList() ;

/*
	// LIGHT5��ʸ���פ�Ȥ餹���ݥåȥ饤�ȡ� ���å�
	GLfloat u_dir[3] ;
	Vector3d dir(spot[0], spot[1], spot[2]) ;
	dir.Unit() ;
	u_dir[0] = dir.X() ;
	u_dir[1] = dir.Y() ;
	u_dir[2] = dir.Z() ;

	glEnable(GL_LIGHT5);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);

//	SetDirectionalLight(GL_LIGHT5, ldir[5], lcol[5]);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 1.0f);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 90.0f);

	glNewList(AllocAList(SPOT_LIGHT_LIST), GL_COMPILE) ;
		SetDirectionalLight(GL_LIGHT5, ldir[5], lcol[5]);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, u_dir);
	glEndList() ;

*/
}

// �ƣ�����Ǻ�ꥹ�Ⱥ���
void MakeMaterialLists()
{
	// Gold
	glNewList(AllocAList(MATERIAL_GOLD), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 1.0f, /*0.95f*/ 2.0f * specular * spec, 128.0f) ; //, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Iron
	glNewList(AllocAList(MATERIAL_IRON), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 1.0f, 2.0f * specular * spec, 64.0f) ; //, IS_SET_M_RATIO, SOLID_M_RATIO) ;
	glEndList() ;


	// �̤��Ǻ��-s 2,3 -r 2 ���ץ����Υ���åɥ��������ѡ�
	glNewList(AllocAList(MATERIAL_SHELL_SOLID), GL_COMPILE) ;
//		if (mode_solid == 13)	// Separate_specular �Τߥϥ��饤�Ȥ⥻�å�
			SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.0f, 0.8f * /* specular * */ spec, 128.0f) ;
//		else
//			SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.0f, 0.0f) ;
	glEndList() ;


	// GLASS,GLASS3 �ϡ����饹���濴�����ݤ�����Τ��ɤ�����ˡ��濴�Ѥ��������Ѥ�
	// Glass
	GLfloat r_shin = 128.0f ; //, amb = 0.0f ; // (GLfloat)NONE ;

	glNewList(AllocAList(MATERIAL_GLASS), GL_COMPILE) ;
//		SetMaterial(GL_FRONT, 0.03f,0.03f,0.03f, 1.0f, 0.0f, sl*0.5f * spec, r_shin, FALSE, 0.0f, amb) ;
		SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.5f * spec, r_shin) ;
	glEndList() ;

	// Glass2
	glNewList(AllocAList(MATERIAL_GLASS2), GL_COMPILE) ;
//		SetMaterial(GL_FRONT, 0.1f,0.1f,0.1f, 1.0f, 0.0f, sl*1.0f * spec, r_shin, FALSE, 0.0f, amb) ;
		SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 1.0f * spec, r_shin) ;
	glEndList() ;

	// Glass4
	glNewList(AllocAList(MATERIAL_GLASS4), GL_COMPILE) ;
//		SetMaterial(GL_FRONT_AND_BACK, 0.1f,0.1f,0.1f, 1.0f, 0.0f, sl*1.0f * spec, r_shin, FALSE, 0.0f, amb) ;
		SetMaterial(GL_FRONT_AND_BACK, 1.0f,1.0f,1.0f, 1.0f, 0.9f, 0.75f * spec, r_shin) ;
	glEndList() ;


	// Metal Red
	glNewList(AllocAList(MATERIAL_METAL_RED), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,0.1f,0.1f, 1.0f, 1.0f, /*0.9f*/ 2.0f * specular * spec, 128.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Metal Blue
	glNewList(AllocAList(MATERIAL_METAL_BLUE), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.5f,0.1f,1.0f, 1.0f, 1.0f, /*0.9f*/ 2.0f * specular * spec, 128.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Metal Green
	glNewList(AllocAList(MATERIAL_METAL_GREEN), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.0f,0.6f,0.0f, 1.0f, 1.0f, /*0.9f*/ 2.0f * specular * spec, 128.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Red Gray
	glNewList(AllocAList(MATERIAL_RED_GRAY), GL_COMPILE) ;
	{
//		if      (reality)              SetMaterial(GL_FRONT, 0.9f,0.5f,0.45f, 1.0f, 0.2f, 0.0f) ;
		if      (reality ==3 || reality == 13) SetMaterial(GL_FRONT, 0.8f, 0.5f, 0.45f, 1.0f, 0.0f, 128.0f) ;
		else if (reality)              SetMaterial(GL_FRONT, 0.8f, 0.5f, 0.45f, 1.0f, 0.0f, 0.0f) ;
		else if (texture < 3 || solid) SetMaterial(GL_FRONT, 1.0f,0.7f,0.7f, 1.0f, 0.7f, 0.7f * specular * spec, 32.0f, IS_SET_M_RATIO, SOLID_M_RATIO) ;
		else                           SetMaterial(GL_FRONT, 0.6f,0.3f,0.3f, 1.0f, 0.7f, 0.8f * spec, 128.0f) ;	// -e 3
	}
	glEndList() ;

	// ʸ���ױǤ�����ѡʥܡ��ɤΥǥ��ƥ������
	if (reflection)
	{
		glNewList(AllocAList(MATERIAL_MIRROR_RED_GRAY), GL_COMPILE) ;
			SetMaterial(GL_FRONT, 1.0f,0.6f,0.6f, 0.7f, 0.2f, 0.0f) ;
//			SetMaterial(GL_FRONT, 1.0f,1.0f,1.0f, 0.8f, 0.2f, 0.0f) ;
		glEndList() ;
	}

	// Specular Iron
	glNewList(AllocAList(MATERIAL_IRON_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.8f,0.8f,0.8f, 0.5f, 1.0f, 3.0f*spec, 2.0f) ;
	glEndList() ;

	// Specular Red
	glNewList(AllocAList(MATERIAL_RED_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,0.1f,0.1f, 0.5f, 1.0f, 3.0f*spec, 2.0f) ;
	glEndList() ;

	// Specular Gold
	glNewList(AllocAList(MATERIAL_GOLD_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 0.5f, 1.0f, 3.0f*spec, 2.0f) ;
	glEndList() ;

	// Specular Blue
	glNewList(AllocAList(MATERIAL_BLUE_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.5f,0.1f,1.0f, 0.5f, 1.0f, 3.0f*spec, 2.0f) ;
	glEndList() ;

	// Specular Green
	glNewList(AllocAList(MATERIAL_GREEN_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.1f,0.6f,0.2f, 0.5f, 1.0f, 3.0f*spec, 2.0f) ;
	glEndList() ;

	// Mat Iron
	glNewList(AllocAList(MATERIAL_MAT_IRON), GL_COMPILE) ;
	{
#ifdef ________SOLID_SHADED_CLOCK
		if (mode_real == 3 || mode_real == 13) SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.0f, 128.0f) ;
		else if (mode_real) SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.0f, 0.0f) ;
//		else if (mode_solid) SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.8f, 1.3f * specular * spec, 32.0f, IS_SET_M_RATIO, SOLID_M_RATIO) ;
		else if (mode_env == 2) SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;	// -e 2
		else SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.8f, 1.3f * specular * spec, 32.0f) ;	// normal
#else	// #ifdef ________SOLID_SHADED_CLOCK
		if (mode_env == 2) SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;	// -e 2
		else SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, 0.8f, 1.3f * specular * spec, 48.0f) ;	// normal
#endif	// #ifdef ________SOLID_SHADED_CLOCK
	}
	glEndList() ;

	// Mat Gold
	glNewList(AllocAList(MATERIAL_MAT_GOLD), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f / pre_s_motion * PRE_S_MOTION_A, 0.8f, 1.3f * specular * spec, 32.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Mat Metal Blue
	glNewList(AllocAList(MATERIAL_MAT_METAL_BLUE), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.5f,0.1f,1.0f, 1.0f, 0.8f, 1.3f * specular * spec, 32.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Mat Metal Green
	glNewList(AllocAList(MATERIAL_MAT_METAL_GREEN), GL_COMPILE) ;
		SetMaterial(GL_FRONT, 0.1f,0.6f,0.2f, 1.0f, 0.8f, 1.3f * specular * spec, 32.0f, IS_SET_M_RATIO, METAL_R_M_RATIO) ;
	glEndList() ;

	// Specular
	GLfloat r_spec = 0.8f ;

	glNewList(AllocAList(MATERIAL_SPECULAR), GL_COMPILE) ;
		SetMaterial(GL_FRONT_AND_BACK, 1.0f,1.0f,1.0f, 1.0f, 1.0f, spec * r_spec, 128.0, FALSE, 0.0f, 0.0f) ;
	glEndList() ;
}

// ɽ�̥��饹�ꥹ�Ⱥ���
void MakeGlassList()
{
	if (clock_glass)
	{
		int div ;
		if (clock_glass == 1) div = 60 ;
		else                  div = clock_div ;

		glNewList(AllocAList(GLASS_LIST), GL_COMPILE) ;
		{
			if (clock_glass == 1)
			{
				if (light)
				{
					nm_polygons += DrawGlass(div, 14.4f, TRUE) ;
				}
				else
					nm_polygons += DrawGlass(div, 14.4f, FALSE) ;
			}
			else
			{
				glTranslatef(0.0f, 0.0f, 2.4f) ;
				if (light)
					nm_polygons += DrawGlass(div, 14.4f, TRUE) ;
				else
					nm_polygons += DrawGlass(div, 14.4f, FALSE) ;
			}
		}
		glEndList() ;

		glNewList(AllocAList(ENV_GLASS_LIST), GL_COMPILE) ;
		{
			// -glass 1 �Ϥ����ؤ���ʤ�
			glTranslatef(0.0f, 0.0f, 2.4f) ;

			if (flag_glassTexCoord)
				nm_polygons += DrawGlass(div, 14.4f, FALSE, sAxis.v, tAxis.v) ;
			else
				nm_polygons += DrawGlass(div, 14.4f, FALSE) ;
		}
		glEndList() ;
	}
}


// ���׳��ꥹ�Ⱥ���
void MakeRoofLists()
{
	if (mode_normal || mode_real >= 2 || mode_solid >= 2 || mode_env >= 2)
	{
		glNewList(AllocAList(ROOF_LIST), GL_COMPILE) ;
		{
			glShadeModel(GL_SMOOTH) ;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK

			// ���Υꥹ�Ȥ�ɬ�פʤΤ� mode_nomal, -s 2,3(13) -r 2(12)
			// ���Τ�����mode_normal �Τߥ���ǡ������
			if (mode_normal && light)
			{
				glCallList(MATERIAL_MAT_IRON) ;
				SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.9f, 2.0f * spec, 32.0f) ;
				glColorMaterial(GL_BACK, GL_SPECULAR) ;
				glEnable(GL_COLOR_MATERIAL) ;
				DrawRoof(clock_div, clock_mesh) ;
				glDisable(GL_COLOR_MATERIAL) ;
			}
			else if (mode_normal && !light)
			{
				glColor3f(1.0f, 1.0f, 0.8f) ;
//				DrawRoof(clock_div, clock_mesh, TRUE) ;
				DrawRoof(clock_div, clock_mesh, FALSE) ;
			}
			else if (mode_env == 2)
			{
//				glCallList(MATERIAL_MAT_IRON) ;
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR / detail_texture_correct_bright, ENV2_MATERIAL_SHININESS) ;
				SetMaterial(GL_BACK, 1.0f  * detail_texture_correct_bright,
									 0.75f * detail_texture_correct_bright,
									 0.05f * detail_texture_correct_bright, 1.0f, 1.0f, ENV2_MATERIAL_SPECULAR / detail_texture_correct_bright, ENV2_MATERIAL_SHININESS) ;	// �Хå��Ǻ�

				if (detail)
					DrawRoof(clock_div, clock_mesh, TRUE, vdsAxis, vdtAxis) ;	// �ǥ��ƥ�����ƥ��������ɸ���� ON
				else
					DrawRoof(clock_div, clock_mesh) ;
			}
			else if (mode_solid)
			{
				// -s 2, 3(13)
				glColor3f(1.0f, 1.0f, 0.8f) ;	// �ե���ȥǥ��ե塼�����顼
//				GLfloat d[] = { 1.0f, 1.0f, 0.8f, 1.0f } ;
//				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;

				// �Хå��Ǻ�
//				glDisable(GL_COLOR_MATERIAL) ;
				SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.4f, 0.8f * spec * specular, 128.0) ;
				DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v) ;	// ����ǡ������ʤ�
//				glEnable(GL_COLOR_MATERIAL) ;
			}
			else
			{
				// -r 2(12)

#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, 0.0f, 0.0f) ; //128.0f) ;
				// �Хå��Ǻ�
				SetMaterial(GL_BACK, 1.0f  * detail_texture_correct_bright,
									 0.75f * detail_texture_correct_bright,
									 0.05f * detail_texture_correct_bright, 1.0f, 0.4f, 0.0f) ; //128.0f) ;

#else	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
				// -r 2(12)
				SetMaterial(GL_FRONT, 1.0f, 1.0f, 0.8f, 1.0f, 0.0f, 0.0f) ;
				// �Хå��Ǻ�
				SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.4f, 0.0f) ;
#endif	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE ... #else

/*
				// ����ǡ������ʤ�
				if (detail == 3)	// && mode_real == 12
					DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v, 1.0f, 2, vdsAxis, vdtAxis) ;	// ���ѥ����ƥ�������
				else
					DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v) ;
*/

				// ����ǡ������ʤ�
				if (detail >= 2)	// && mode_real == 12
					DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v, 1.0f, 1, vdsAxis, vdtAxis) ;	// �ޥ������åɥƥ�������
				else
					DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v) ;
			}

#else	// #ifdef ________SOLID_SHADED_CLOCK

			glCallList(MATERIAL_MAT_IRON) ;
			if (!reality)
			{
				if (mode_normal)
					SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.95f, 2.0f * spec, 32.0f) ;
				else if (mode_solid == 13)
					SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.5f, 2.0f * spec, 32.0f) ;
				else
					SetMaterial(GL_BACK, 1.0f,0.75f,0.05f, 1.0f, 0.5f, 0.0) ;

				glColorMaterial(GL_BACK, GL_SPECULAR) ;
				glEnable(GL_COLOR_MATERIAL) ;
				if (flag_shadeTexCoord)
//					DrawRoof(clock_div, clock_mesh, (mode_solid != 3), sAxis.v, tAxis.v) ;
					DrawRoof(clock_div, clock_mesh, /*(mode_solid != 3)*/ !mode_solid, sAxis.v, tAxis.v) ;
				else
					DrawRoof(clock_div, clock_mesh) ;

//				MessageBox(NULL, String("mode_solid: ") + IntToString(mode_solid) + "  mode_normal: " + IntToString(mode_normal), "GLU Error", MB_OK | MB_ICONSTOP) ;

				glDisable(GL_COLOR_MATERIAL) ;
			}
			else	// -r
			{
				// -r �Υ���ǡ������Ϥʤ��������ɤ���
				SetMaterial(GL_BACK, 0.6f,0.45f,0.03f, 1.0f, 0.0f, 0.0f) ;
				DrawRoof(clock_div, clock_mesh, FALSE, sAxis.v, tAxis.v) ;
			}

#endif	// #ifdef ________SOLID_SHADED_CLOCK

		}
		glEndList() ;
	}

	if (!(mode_normal || mode_solid == 2 || mode_solid >= 10))
	{
		glNewList(AllocAList(ENV_ROOF_LIST), GL_COMPILE) ;
			glShadeModel(GL_SMOOTH) ;
			if (flag_modulateTexCoord)
				DrawRoof(clock_div, clock_mesh, !(filterTest/* || orgImageFlag || maxTriTest*/), sAxis.v, tAxis.v) ;	// -e 3 �λ��Σ����ܤ������ -r 1 �� -r 2 ������ܤȥ��ꥹ����ϡ������ؤ� ���ʤ�
			else if (mode_env == 1 && detail)
				DrawRoof(clock_div, clock_mesh, !filterTest, vdsAxis, vdtAxis) ;	// -e 1 -D
			else
				DrawRoof(clock_div, clock_mesh, !((env == 1 && filterTest) || env == 3 || reality || crystal)) ;	// -e 3 �λ��Σ����ܤ������ -r 1 �� -r 2 ������ܤȥ��ꥹ���� �ϥ���ǡ������ʤ�
		glEndList() ;
	}

	// �����Х���ץ���ѤΥꥹ��
	// default_motion, default_anti, default_field �����ο������Ѱ�
	// ���ͤ��Ѳ������ʤ������
	// �� = 1.0f / (n + 1)
	if (mode_solid == 1 || mode_env == 1)
	{
		int max_sample = Max(default_anti, Max(default_motion, default_field)) ;
		for (int n = 0 ; n < max_sample ; n ++)
		{
			glNewList(AllocAList(ENV_ROOF_OVER_LIST[n]), GL_COMPILE) ;
			{
				GLfloat alpha = 1.0f / (n + 1) ;

				glShadeModel(GL_SMOOTH) ;
				if (flag_modulateTexCoord)
					DrawRoof(clock_div, clock_mesh, !filterTest /*&& !orgImageFlag*/, sAxis.v, tAxis.v, alpha) ;	// �ե��륿�ƥ��Ȥޤ��� -ORG ���Τߥ���ǡ������ʤ�
				else if (mode_env == 1 && detail)
					DrawRoof(clock_div, clock_mesh, !filterTest, vdsAxis, vdtAxis, alpha) ;	// -e -D
				else
					DrawRoof(clock_div, clock_mesh, !filterTest, NULL, NULL, alpha) ;
			}
			glEndList() ;
		}
	}

	// �ǥ��ƥ�����ƥ��������ѥꥹ��
	// detail �ǡ��ޥ���ƥ������㵡ǽ��Ȥ��ʤ��Ȥ�
	if (detail == 1 || detail == 2)
//	if (detail)
	{
		glNewList(AllocAList(ROOF_DETAILTEX_LIST), GL_COMPILE) ;
			DrawRoof(clock_div, clock_mesh, FALSE, vdsAxis, vdtAxis) ;
		glEndList() ;
	}
}


// �̥ꥹ�������˴ޤޤ��������ʬ����
void DrawGoldenObjectInShellList()
{
	if (gold_depth < 0)
		return ;

	if (!mode_real)
	{
		if (!g_smooth)
			glShadeModel(GL_FLAT) ;

		if (mode_solid)
		{
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
//			GLfloat d[] = { 1.0f, 0.75f, 0.05f, 1.0f } ;
//			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;
			glColor3f(1.0f, 0.75f, 0.05f) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

			SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.0f, 2.0f * spec *specular, 128.0f) ;
#endif	// #ifdef ________SOLID_SHADED_CLOCK

			DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;
		}
		else if (mode_normal && light)
		{
//			glCallList(MATERIAL_GOLD) ;
			SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.95f, 2.0f * spec, 128.0f) ;
			DrawGoldenObjects(14.6f, n_golden) ;
		}
		else if (mode_normal && !light)
		{
			glColor3f(1.0f, 0.75f, 0.05f) ;
			DrawGoldenObjects(14.6f, n_golden) ;
		}
		else	// -e 2
		{
			// -e 2 �ξ�硢
			// ����ƥ������㥻�å�
//			SetMaterial(GL_FRONT, 1.0f,0.7f,0.05f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;
			SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
								  0.7f * detail_texture_correct_bright,
								  0.05f * detail_texture_correct_bright, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR / detail_texture_correct_bright, ENV2_MATERIAL_SHININESS) ;

			// detail ���� glActiveTexture(GL_TEXTURE1) �ˤʤäƤ���ɬ�פ���
			glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;

			if (detail)
				DrawGoldenObjects(14.6f, n_golden, vdsAxis, vdtAxis) ;	// �ǥ��ƥ�����ƥ��������ɸ���� ON
			else
				DrawGoldenObjects(14.6f, n_golden) ;
		}
	}
}


// ���׳̥ꥹ�Ⱥ���
void MakeShellLists()
{
	// �����ꥹ�Ⱥ���
//	MakeGoldenObjectList() ;

	if (mode_solid != 1 && mode_env != 1 && !mode_crystal)
	{
		glNewList(AllocAList(SHELL_LIST), GL_COMPILE) ;
		{
#ifdef ________SOLID_SHADED_CLOCK

			if (mode_normal && light)
			{
				glCallList(MATERIAL_MAT_IRON) ;
			}
			else if (mode_env == 2)
			{
//				SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR / detail_texture_correct_bright, ENV2_MATERIAL_SHININESS) ;
			}
			else if (mode_normal && !light)
			{
				glColor3f(1.0f, 1.0f, 0.8f) ;
			}
			else if (mode_solid)
			{
				// -s 2, 3(13)
//				GLfloat d[] = { 1.0f, 1.0f, 0.8f, 1.0f } ;
//				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;
				glColor3f(1.0f, 1.0f, 0.8f) ;	// �ե���ȥǥ��ե塼�����顼
			}
			else
			{
				// -r 2(12)
#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, 0.0f, 0.0f) ;
#else	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE

				SetMaterial(GL_FRONT, 1.0f, 1.0f, 0.8f, 1.0f, 0.0f, 0.0f) ;
#endif	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE ... #else
			}

#else	// #ifdef ________SOLID_SHADED_CLOCK

			glCallList(MATERIAL_MAT_IRON) ;
#endif	// #ifdef ________SOLID_SHADED_CLOCK

			if (flag_shadeTexCoord)
			{
/*
				if (detail == 3)
				{
					// ���ѥ����ƥ�������ξ��
					// �ƥ��������˥åȣ��˥ǥ��ƥ�����ƥ��������ɸ���å�
					DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, sAxis.v, tAxis.v, TRUE, 2, vdsAxis, vdtAxis) ;
				}
*/
				if (detail >= 2)
				{
					// �ǥ��ƥ�����ƥ��������ޥ���ƥ�������ǽ���������
					// �ƥ��������˥åȣ��˥ǥ��ƥ�����ƥ��������ɸ���å�
					DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, sAxis.v, tAxis.v, TRUE, 1, vdsAxis, vdtAxis) ;
				}
				else
					DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, sAxis.v, tAxis.v) ;
			}
			else if (mode_env == 2 && detail)
			{
				// �ǥ��ƥ�����ƥ��������ɸ���å� ON
#ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, NULL, NULL, TRUE, 1, vdsAxis, vdtAxis) ;

#else	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, vdsAxis, vdtAxis) ;

#endif	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV ... #else
			}
			else
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal) ;
//				WineGlass::DrawPolyhedron(clock_div, 12.0f, flag_glNormal) ;


			if (!mode_real)
				DrawGoldenObjectInShellList() ;
/*
			{
				if (!local && mode_normal)
					glShadeModel(GL_FLAT) ;	// !local && !texture �λ��ϥե�å�

				if (mode_solid)
				{
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
//					GLfloat d[] = { 1.0f, 0.75f, 0.05f, 1.0f } ;
//					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;
					glColor3f(1.0f, 0.75f, 0.05f) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.0f, 2.0f * spec *specular, 128.0f) ;
#endif	// #ifdef ________SOLID_SHADED_CLOCK

					DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;
				}
				else if (mode_normal) // normal || -e 3?
				{
//					glCallList(MATERIAL_GOLD) ;
					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.95f, 2.0f * spec, 128.0f) ;
					DrawGoldenObjects(14.6f, n_golden) ;
				}
				else	// -e 2
				{
					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.8f, 0.0f) ;
					DrawGoldenObjects(14.6f, n_golden) ;
				}
			}
*/
		}
		glEndList() ;

		// ɽ�����Ѵʰץ�ǥ�
		glNewList(AllocAList(SHELL_LIST_CULL), GL_COMPILE) ;
		{
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK

			if (mode_normal && light)
				glCallList(MATERIAL_MAT_IRON) ;
			else if (mode_env == 2)
			{
//				SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR / detail_texture_correct_bright, ENV2_MATERIAL_SHININESS) ;
			}
			else if (mode_normal && !light)
				glColor3f(1.0f, 1.0f, 0.8f) ;
			else if (mode_solid)
			{
				// -s 2, 3(13)
//				GLfloat d[] = { 1.0f, 1.0f, 0.8f, 1.0f } ;
//				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;
				glColor3f(1.0f, 1.0f, 0.8f) ;	// �ե���ȥǥ��ե塼�����顼
			}
			else
			{
				// -r 2(12)
#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
				SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
									  1.0f * detail_texture_correct_bright,
									  0.8f * detail_texture_correct_bright, 1.0f, 0.0f, 0.0f) ;
#else	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
				SetMaterial(GL_FRONT, 1.0f, 1.0f, 0.8f, 1.0f, 0.0f, 0.0f) ;
#endif	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE ... #else
			}

#else	// #ifdef ________SOLID_SHADED_CLOCK

			glCallList(MATERIAL_MAT_IRON) ;

#endif	// #ifdef ________SOLID_SHADED_CLOCK

			if (flag_shadeTexCoord)
			{
//				if (detail == 3)
				if (detail >= 2)
				{
					// ���ѥ����ƥ�������ξ��
					// �ƥ��������˥åȣ��˥ǥ��ƥ�����ƥ��������ɸ���å�
					DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, sAxis.v, tAxis.v, TRUE, 1, vdsAxis, vdtAxis) ;
				}
				else
					DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, sAxis.v, tAxis.v) ;
			}
			else if (mode_env == 2 && detail)
			{
				// �ǥ��ƥ�����ƥ��������ɸ���å� ON
#ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, NULL, NULL, TRUE, 1, vdsAxis, vdtAxis) ;

#else	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, vdsAxis, vdtAxis) ;

#endif	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV ... #else
			}
			else
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal) ;

			if (!mode_real)
				DrawGoldenObjectInShellList() ;
/*
			{
				if (!local && mode_normal)
					glShadeModel(GL_FLAT) ;	// !local && !texture �λ��ϥե�å�

				if (mode_solid)
				{
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
//					GLfloat d[] = { 1.0f, 0.75f, 0.05f, 1.0f } ;
//					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, d) ;
					glColor3f(1.0f, 0.75f, 0.05f) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.0f, 2.0f * spec *specular, 128.0f) ;
#endif	// #ifdef ________SOLID_SHADED_CLOCK

					DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;

				}
				else if (mode_normal) // normal || -e 3?
				{
//					glCallList(MATERIAL_GOLD) ;
					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.95f, 2.0f * spec, 128.0f) ;
					DrawGoldenObjects(14.6f, n_golden) ;
				}
				else	// -e 2
				{
					SetMaterial(GL_FRONT, 1.0f,0.75f,0.05f, 1.0f, 0.8f, 0.0f) ;
					DrawGoldenObjects(14.6f, n_golden) ;
				}
			}
*/
		}

		glEndList() ;
	}

	// ���ξ��ϡ��̤ȶ������̤Υꥹ�Ȥˤ���ʤ��������򥻥åȤ���Ϣ³�ǸƤӽФ���
	if (!(mode_normal || mode_solid == 2 || mode_solid >= 10))
	{
		glNewList(AllocAList(ENV_SHELL_LIST), GL_COMPILE) ;
		{
			if (flag_modulateTexCoord)
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, sAxis.v, tAxis.v) ;	// -solid 1 �ξ��Τߡ�ˡ���ϰ�������
			else if (mode_env == 1 && detail)
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal, vdsAxis, vdtAxis) ;	// -e 1 -D
			else
				DrawShell(clock_div, clock_mesh, FALSE, flag_glNormal) ;	// -solid 1 �ξ��Τߡ�ˡ���ϰ�������

			// -solid 1 �� filterTest �ξ��ϡ���°���֥�����ޤ��
			if (mode_solid == 1 && filterTest)
				DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;	// �ץƥ��������ɸ
		}
		glEndList() ;

		// ɽ�����ʰץ�ǥ�
		glNewList(AllocAList(ENV_SHELL_LIST_CULL), GL_COMPILE) ;
		{
			if (flag_modulateTexCoord)
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, sAxis.v, tAxis.v) ;	// -solid 1 �ξ��Τߡ�ˡ���ϰ�������
			else if (mode_env == 1 && detail)
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal, vdsAxis, vdtAxis) ;	// -e 1 -D
			else
				DrawShell(clock_div, clock_mesh, TRUE, flag_glNormal) ;	// -solid 1 �ξ��Τߡ�ˡ���ϰ�������

			// -solid 1 �� filterTest �ξ��ϡ���°���֥�����ޤ��
			if (mode_solid == 1 && filterTest)
				DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;	// �ץƥ��������ɸ
		}
		glEndList() ;

		if (!(mode_solid == 1 && filterTest))
		{
			glNewList(AllocAList(ENV_GOLDEN_OBJECTS_LIST), GL_COMPILE) ;
			{
				if (!local && !texture) glShadeModel(GL_FLAT) ;	// !local && !texture �λ��ϥե�å�
				if (flag_modulateTexCoord)
					DrawGoldenObjects(14.6f, n_golden, sAxis.v, tAxis.v) ;
				else if (mode_env == 1 && detail)
					DrawGoldenObjects(14.6f, n_golden, vdsAxis, vdtAxis) ;
				else
					DrawGoldenObjects(14.6f, n_golden) ;
			}
			glEndList() ;
		}
	}

	if (reflection)
	{
		// ʸ���פ˱Ǥ������Ϥ��ɥꥹ�Ⱥ���
		glNewList(AllocAList(MIRROR_WALL_LIST), GL_COMPILE) ;
		{
			SetMaterial(GL_FRONT, 1.0f,1.0f,0.8f, 0.5f, 0.8f, 1.3f * specular * spec, 1.0f, TRUE, 0.2f) ;

			if (donotUSe_AlphaGouraud)
			{
//				DrawMirrorWall(clock_div, FALSE) ; // ����ե��������ʤ���RIVA, PERMEDIA2 �ʤɡ�
				DrawMirrorWall(clock_div, FALSE, 0.0f, 0.0f, sAxis.v, tAxis.v) ; // ����ե��������ʤ���RIVA, PERMEDIA2 �ʤɡ�
			}
			else
			{
				glColorMaterial(GL_FRONT, GL_DIFFUSE) ;
				glEnable(GL_COLOR_MATERIAL) ;
//				DrawMirrorWall(clock_div, TRUE, 0.8f, 0.0f) ; // ����ե�������
				DrawMirrorWall(clock_div, TRUE, 0.8f, 0.0f, sAxis.v, tAxis.v) ; // ����ե�������
				glDisable(GL_COLOR_MATERIAL) ;
			}
		}
		glEndList() ;

		glNewList(AllocAList(ENV_MIRROR_WALL_LIST), GL_COMPILE) ;
			DrawMirrorWall(clock_div, FALSE) ;
		glEndList() ;
	}

	// detail �ǡ��ޥ���ƥ������㤬����˥åȻȤ��ʤ��Ȥ�
	if (detail == 1 || detail == 2)
	{
		glNewList(AllocAList(SHELL_DETAILTEX_LIST), GL_COMPILE) ;
			DrawShell(clock_div, clock_mesh, FALSE, FALSE, vdsAxis, vdtAxis, FALSE) ;
		glEndList() ;

		glNewList(AllocAList(SHELL_DETAILTEX_LIST_CULL), GL_COMPILE) ;
			DrawShell(clock_div, clock_mesh, TRUE, FALSE, vdsAxis, vdtAxis, FALSE) ;
		glEndList() ;
	}
}


// ʸ���ץꥹ�Ⱥ���
void MakeBoardLists()
{
	int div = clock_div ;
//	if (envOpenGL == RIVA_TNT_FIX && div > 58)
//		div = 58 ;

//	MakeClockPointList() ;	// ʸ���׾�ξ�����γ��
//	MakeMList() ;			// ��

	glNewList(AllocAList(BOARD_LIST), GL_COMPILE) ;
	{
		glPushMatrix() ;
		{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK

			if (mode_real)
			{
				if (orgImageFlag)
				{
#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
					SetMaterial(GL_FRONT, 1.0f * detail_texture_correct_bright,
										  1.0f * detail_texture_correct_bright,
										  1.0f * detail_texture_correct_bright, 1.0f, 0.0f, 0.0f) ;	//  -ORG

#else	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
					SetMaterial(GL_FRONT, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f) ;	//  -ORG

#endif	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE ... #else
				}
				else
				{
#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
					SetMaterial(GL_FRONT, 0.8f  * detail_texture_correct_bright,
										  0.5f  * detail_texture_correct_bright,
										  0.45f * detail_texture_correct_bright, 1.0f, 0.0f, 0.0f) ;

#else	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
					SetMaterial(GL_FRONT, 0.8f, 0.5f, 0.45f, 1.0f, 0.0f, 0.0f) ;

#endif	// #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE ... #else
				}
			}
			else if (mode_solid)
			{
				if (orgImageFlag)
					glColor3f(1.0f, 1.0f, 1.0f) ;	// -s 2, 3
				else
					glColor3f(0.8f, 0.5f, 0.45f) ;	// -s 2, 3 -ORG
			}
			else if (mode_env == 2)
				SetMaterial(GL_FRONT, 0.75f,0.45f,0.45f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;	// -e 2
//				SetMaterial(GL_FRONT, 1.0f,0.6f,0.6f, 1.0f, ENV2_MATERIAL_METAL_RATIO, ENV2_MATERIAL_SPECULAR, ENV2_MATERIAL_SHININESS) ;	// -e 2

//			else if (mode_env == 3)
//				SetMaterial(GL_FRONT, 0.6f,0.3f,0.3f, 1.0f, 0.7f, 0.8f * spec, 128.0f) ;	// -e 3
			else if (mode_normal && light)	// mode_normal
				SetMaterial(GL_FRONT, 1.0f,0.7f,0.7f, 1.0f, 0.7f, 0.7f * spec, 32.0f) ;		// normal
			else	// mode_normal && !light
				glColor3f(1.0f,0.7f,0.7f) ;		// normal

#else	// #ifdef ________SOLID_SHADED_CLOCK

			glCallList(MATERIAL_RED_GRAY) ;

#endif	// #ifdef ________SOLID_SHADED_CLOCK

//			if (!local) glShadeModel(GL_SMOOTH) ;
			glShadeModel(GL_SMOOTH) ;

			if (flag_shadeTexCoord)
			{
/*
				if (detail == 3)
				{
					if (reflection)
					{
						// -R ���ϡ��ƥ������㣱�˥ǥ��ƥ�����򥻥å�
						DrawBoard(div, 15.0f, flag_glNormal, sAxis.v, tAxis.v, 1, vdsAxis, vdtAxis) ;
					}
					else
					{
						// �Ǥꤳ�ߤʤ����ϡ��ƥ������㣲�˥ǥ��ƥ�����򥻥å�
						DrawBoard(div, 15.0f, flag_glNormal, sAxis.v, tAxis.v, 2, vdsAxis, vdtAxis) ;
					}
				}
*/
				if (detail >= 2)
				{
					// �ޥ���ƥ�������ǥǥ��ƥ�����ƥ�����������������
					// �ƥ������㣱�˥ǥ��ƥ����륻�å�
					DrawBoard(div, 15.0f, flag_glNormal, sAxis.v, tAxis.v, 1, vdsAxis, vdtAxis) ;
				}
				else
				{
					// �ǥ��ƥ�����ʤ��λ��ϥƥ��������ɸ������Ū�ʻ���ϥ���åɤΤ�
					DrawBoard(div, 15.0f, flag_glNormal, sAxis.v, tAxis.v) ;
				}
			}
			else if (mode_env == 2 && detail)
			{
				// �ǥ��ƥ�����ƥ��������ɸ���å� ON
#ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawBoard(div, 15.0f, flag_glNormal, NULL, NULL, 1, vdsAxis, vdtAxis) ;

#else	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
				DrawBoard(div, 15.0f, flag_glNormal, vdsAxis, vdtAxis) ;

#endif	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV ... #else
			}
			else
				DrawBoard(div, 15.0f, flag_glNormal) ;

			// �Ǥ���߻��ϥǥ��ƥ�������̥ꥹ�Ȥ�
			if (!reflection)
			{
				glPushMatrix() ;
					glTranslatef(0.0f,5.0f,0.0f) ;
//					glCallList(M_LIST) ;
				if (flag_shadeTexCoord)
					DrawM(1.0f, 1.0f, 1.0f, sAxis.v, tAxis.v) ;
				else if (mode_env == 2 && detail)
				{
				// �ǥ��ƥ�����ƥ��������ɸ���å� ON
#ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
					DrawM(1.0f, 1.0f, 1.0f, vdsAxis, vdtAxis) ;		// DETAIL_TEXTURE_BLEND_AFTER_ENV ��Ȥ��ʤ鿧������ɬ��

#else	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV
					DrawM(1.0f, 1.0f, 1.0f, vdsAxis, vdtAxis) ;

#endif	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV ... #else
				}
				else
					DrawM(1.0f, 1.0f, 1.0f) ;

				glPopMatrix() ;

//				if (!local) glShadeModel(GL_SMOOTH) ;
				glTranslatef(0.0f,0.0f,0.0001f) ;
				if (clock_point)
				{
					if (flag_shadeTexCoord)
						DrawClockPoints(radius_ratio * BITS_RADIUS, sAxis.v, tAxis.v) ;
					else if (mode_env == 2 && detail)
						DrawClockPoints(radius_ratio * BITS_RADIUS, vdsAxis, vdtAxis) ;
					else
						DrawClockPoints(radius_ratio * BITS_RADIUS) ;
				}
	       	}
		}
		glPopMatrix() ;
	}
	glEndList() ;

	glNewList(AllocAList(ENV_BOARD_LIST), GL_COMPILE) ;
	{
		glPushMatrix() ;
		{
			if (!local) glShadeModel(GL_SMOOTH) ;
			if (flag_modulateTexCoord)
				DrawBoard(div, 15.0f, flag_glNormal, sAxis.v, tAxis.v) ;
			else if (mode_env == 1 && detail)
				DrawBoard(div, 15.0f, flag_glNormal, vdsAxis, vdtAxis) ;	// -e 1 -D
			else
				DrawBoard(div, 15.0f, flag_glNormal) ;

			// �Ǥ���߻��ϥǥ��ƥ�������̥ꥹ�Ȥ�
			if (!reflection)
			{
				glPushMatrix() ;
					glTranslatef(0.0f,5.0f,0.0f) ;
//					glCallList(M_LIST) ;
					if (flag_modulateTexCoord)
						DrawM(1.0f, 1.0f, 1.0f, sAxis.v, tAxis.v) ;
					else if (mode_env == 1 && detail)
						DrawM(1.0f, 1.0f, 1.0f, vdsAxis, vdtAxis) ;	// -e 1 -D
					else
						DrawM(1.0f, 1.0f, 1.0f) ;
				glPopMatrix() ;

//				if (!local) glShadeModel(GL_SMOOTH) ;
				glTranslatef(0.0f,0.0f,0.0001f) ;
				if (clock_point)
				{
					if (flag_modulateTexCoord)
						DrawClockPoints(radius_ratio * BITS_RADIUS, sAxis.v, tAxis.v) ;
					else if (mode_env == 1 && detail)
						DrawClockPoints(radius_ratio * BITS_RADIUS, vdsAxis, vdtAxis) ;	// -e 1 -D
					else
						DrawClockPoints(radius_ratio * BITS_RADIUS) ;
				}
			}
		}
		glPopMatrix() ;
	}
	glEndList() ;

	// �Ǥ���߻��ϥǥ��ƥ�������̥ꥹ�Ȥ�
	if (reflection)	// -r 1, 2(12) �Τ�
	{
		glNewList(AllocAList(BOARD_DETAIL_LIST), GL_COMPILE) ;
		{

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef ________SOLID_SHADED_CLOCK
			SetMaterial(GL_FRONT, 0.8f, 0.5f, 0.45f, 1.0f, 0.0f, 0.0f) ;

#else	// #ifdef ________SOLID_SHADED_CLOCK

			glCallList(MATERIAL_RED_GRAY) ;

#endif	// #ifdef ________SOLID_SHADED_CLOCK

			glPushMatrix() ;
				glTranslatef(0.0f,5.0f,0.0f) ;
//				glCallList(M_LIST) ;
				if (mode_real >= 10)
					DrawM(1.0f, 1.0f, 1.0f, sAxis.v, tAxis.v) ;
				else
					DrawM(1.0f, 1.0f, 1.0f) ;
			glPopMatrix() ;

//			if (!local) glShadeModel(GL_SMOOTH) ;
			glTranslatef(0.0f,0.0f,0.0001f) ;
			if (clock_point)
			{
				if (mode_real >= 10)
					DrawClockPoints(radius_ratio * BITS_RADIUS, sAxis.v, tAxis.v) ;
				else
					DrawClockPoints(radius_ratio * BITS_RADIUS) ;
			}
		}
		glEndList() ;

		glNewList(AllocAList(ENV_BOARD_DETAIL_LIST), GL_COMPILE) ;
		{
			glPushMatrix() ;
				glTranslatef(0.0f,5.0f,0.0f) ;
//				glCallList(M_LIST) ;
				DrawM(1.0f, 1.0f, 1.0f, sAxis.v, tAxis.v) ;
			glPopMatrix() ;

//			if (!local) glShadeModel(GL_SMOOTH) ;
			glTranslatef(0.0f,0.0f,0.0001f) ;
			if (clock_point) DrawClockPoints(radius_ratio * BITS_RADIUS, sAxis.v, tAxis.v) ;
		}
		glEndList() ;
	}

	// �ǥ��ƥ�����ƥ��������ѥꥹ��
	// detail �ǡ��ޥ���ƥ������㵡ǽ��Ȥ��ʤ��Ȥ�
	if (detail == 1 || detail == 2)
	{
		glNewList(AllocAList(BOARD_DETAILTEX_LIST), GL_COMPILE) ;
		{
			glPushMatrix() ;
				DrawBoard(div, 15.0f, FALSE, vdsAxis, vdtAxis) ;
			glPopMatrix() ;
		}
		glEndList() ;
	}
}

// �Ǥ�����Ѥ� s, t ��ɸ�Ϥ򥻥å�
void SetMirrorTexturePlaneST()
{
	glTexGenfv(GL_S, GL_OBJECT_PLANE, ms.v);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, mt.v);
}


// mipmapTest �ѥߥåץޥå׺��������å�
// �ƥߥåץޥåפ�����˽�������
void SetTestMipMapImage(GLint format = 3)
{
	int maxSize = maxTextureSize ;
	if (maxSize > defaultTextureSize)
		maxSize = defaultTextureSize ;

	int maxLevel = (int)Log(2.0, maxSize) ;
	for (int level = 0 ; level <= maxLevel ; level ++)
	{
		int size = Pow(2, (maxLevel - level)) ;	// �ƥ�٥�Υƥ������㥵����

		TRUEIMAGE *image = NewDefaultImage(size) ;

		// ���������ƥ�������� OpenGL �˥��å�
		glTexImage2D(GL_TEXTURE_2D, level, format, size, size,
					 0, GL_RGB, GL_UNSIGNED_BYTE, image->dataptr) ;

		// ��������
		freeimage(image) ;
	}
}


// �ߥåץޥå׺��������å�
// ���������ƥ�٥���˿����ѹ�
// image ��ͽ���Ŭ�ʥ������˥������뤵��Ƥ���ɬ�פ���
// �ޤ���image �Υե����ޥåȤ�ɬ�� RGB 24 bits (GL_RGB, GL_UNSIGNED_BYTE)
void SetTestMipMapImage(TRUEIMAGE *image, GLint format = 3)
{
	int width, height, maxLevel ;
	int err ;
	TRUEIMAGE *img[16] ;	// ���� 16 ��٥�

	width  = image->width ;
	height = image->height ;

	maxLevel = 0 ;
	img[0] = CreateCopyImage(image) ;

	while (TRUE)
	{
		if (width ==1 && height == 1) break ;
		maxLevel ++ ;

		width  = (width  > 1 ? width  / 2 : 1) ;
		height = (height > 1 ? height / 2 : 1) ;

		img[maxLevel] = (TRUEIMAGE *)malloc(sizeof(TRUEIMAGE)) ;
		img[maxLevel]->dataptr = (unsigned char *)malloc(sizeof(unsigned char) * (width + 4) * height * 3) ;

		img[maxLevel]->width  = width ;
		img[maxLevel]->height = height ;

		if (err = gluScaleImage(GL_RGB, img[maxLevel - 1]->width, img[maxLevel - 1]->height,
								GL_UNSIGNED_BYTE, img[maxLevel - 1]->dataptr, width, height,
								GL_UNSIGNED_BYTE, img[maxLevel]->dataptr)
			)
		{
			EndProc() ;
#ifdef WIN32
			MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
			errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
			freeimage(img[maxLevel]) ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}

	// �ƥ�٥�Υƥ�������ο����ѹ���
	// OpenGL �˥��å�
	for (int level = 0 ; level <= maxLevel ; level ++)
	{
		float r, g, b ;
		int c = 7 - (level % 7) ;

		r = ((c & (1     )) ? 1.0f : 0.6f) ;
		g = ((c & (1 << 1)) ? 1.0f : 0.7f) ;
		b = ((c & (1 << 2)) ? 1.0f : 0.5f) ;
//		r = ((c & (1     )) ? 1.0f : 0.3f) ;
//		g = ((c & (1 << 1)) ? 1.0f : 0.4f) ;
//		b = ((c & (1 << 2)) ? 1.0f : 0.2f) ;

//		r = g = b = 1.0f ;

//		ModulateRGB(img[level], r, g, b,  0.0f, 0,0, img[level]->width / 2, img[level]->height / 2) ;
		ModulateRGB(img[level], r, g, b) ;

		glTexImage2D(GL_TEXTURE_2D, level, format, img[level]->width, img[level]->height,
					 0, GL_RGB, GL_UNSIGNED_BYTE, img[level]->dataptr) ;
	}

	// ����������������
	for (int level = 0 ; level <= maxLevel ; level ++)
		freeimage(img[level]) ;
}


// ���᡼���ơ��֥뤫��ƥ�������򥻥å�
void SetTextureImage(TRUEIMAGE *image)
{
	GLint  err ;

	// -FTEST, -MTEST �ϥƥ����ѥߥåץޥåפ�����
	// �¹���˥ե��륿���ѹ����ǽ�ˤ��뤿���-BENCH ��ʻ�ѡ�
	if (mipmapTest)
	{
		// �ߥåץޥåץƥ��Ȼ�
		SetTestMipMapImage(tex_internal_format) ;
	}
	else if (filterTest)
	{
		// �ե��륿�ƥ��Ȼ�
		// �����ƥ������㥵�����ޤ�
		SetTestMipMapImage(image, tex_internal_format) ;
	}
//	else if (gl_min_filter != GL_NEAREST && gl_min_filter != GL_LINEAR)
	else	// 2000-01-18	���٤Ƥμ¹Ԥǥߥåץޥåפ�������ưŪ�ڤ��ؤ���ǽ�ˤ��뤿���
	{
		if (err = gluBuild2DMipmaps(GL_TEXTURE_2D, tex_internal_format, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->dataptr))
		{
			EndProc() ;
#ifdef WIN32
			MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
			errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}
/*	// 2000-01-18	���٤Ƥμ¹Ԥǥߥåץޥåפ�������ưŪ�ڤ��ؤ���ǽ�ˤ��뤿���
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bits, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->dataptr) ;
	}
*/
}

// �ƥ��������ɸ���åȡ�-e,-r ���ץ�����ѴĶ��ƥ��������ɸ�����
void TextureCoordListProc(GLfloat x, GLfloat y, GLfloat w, GLfloat h, int fake = FALSE)
{
	glLoadIdentity() ;

	glTranslatef(x, y, 0.0f) ;
	glScalef(w, h, 1.0f) ;

	if (fake)	// �ե������Ķ��ޥå�
	{
//		glScalef(0.5f, 0.5f, 0.5f) ;
//		glTranslatef(1.0f, 1.0f, 1.0f) ;
		ScaleTranslateMatrixForFake() ;
	}
}

// �ƥ�������κ�ɸ�������ؿ������åȡ�-r ���ץ�����ѥƥ��������ڤ��ؤ��ꥹ�ȡ�
// x,y, w,h OpenGL-1.0 ��ͭ���ƥ����������
// texCoordFlag	 tex_mode == GL_OBJECT_LINEAR �λ����ƥ��������ɸ�ΰ�ư����ž��Ԥ�����
void TextureListProc(GLenum tex_mode = GL_SPHERE_MAP, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat w = 1.0f, GLfloat h = 1.0f, int texCoordFlag = TRUE)
{
	if (tex_mode == GL_OBJECT_LINEAR)
	{
		// ����åɥƥ��������Ѥ˹���򥻥å�
		glMatrixMode(GL_TEXTURE) ;

		if (has_texture_object)
			glLoadIdentity() ;
		else
			TextureCoordListProc(x,y, w,h) ;

		if (texCoordFlag)
		{
			glTranslatef(0.5f, 0.5f, 0.0f) ;
			if (tex_rotate != 0.0f)
				glRotatef(tex_rotate, 0.0f,0.0f,1.0f) ;
//			glScalef(1.0f / s_scale, 1.0f / t_scale, 1.0f) ;
		}
	}
	else if (tex_mode == GL_SPHERE_MAP)
	{
		// �Ķ��ƥ��������Ѥ˹���򥻥å�
		glMatrixMode(GL_TEXTURE) ;

		if (has_texture_object)
		{
			glLoadIdentity() ;

			if (fakeSphereMap)	// �ե������Ķ��ޥå�
			{
//				glScalef(0.5f, 0.5f, 0.5f) ;
//				glTranslatef(1.0f, 1.0f, 1.0f) ;
				ScaleTranslateMatrixForFake() ;
			}
		}
		else
		{
			// not supported texture_object
			TextureCoordListProc(x,y, w,h, fakeSphereMap) ;

/*
			if (fakeSphereMap)	// �ե������Ķ��ޥå�
			{
				glScalef(0.5f, 0.5f, 0.5f) ;
				glTranslatef(1.0f, 1.0f, 1.0f) ;
			}
*/
		}
	}
}


// �ƥ�����������ñ�̲�
void ClearTextureMatrix()
{
	glMatrixMode(GL_TEXTURE) ;
	glLoadIdentity() ;
}


// �ƥ������㶦������
void SetTexture(GLenum texBlendFunc = GL_MODULATE)
{
//	glPixelStorei(GL_UNPACK_ALIGNMENT,1) ;

	UpdateOpenGLTextureFilter() ;
	UpdateOpenGLAnisotropy(maxAnisotropy) ;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texBlendFunc) ;

//	glEnable(GL_TEXTURE_2D) ;
}


void TextureProc(TRUEIMAGE *image, GLenum texBlendFunc = GL_MODULATE)
{
	// �ƥ������㡦���֥������Ȥ����Ѳ�ǽ�ʾ��
	// �ƥ�����������ϥХ������˹Ԥ�
	if (has_texture_object)
		SetTexture(texBlendFunc) ;

//#ifdef GL_VERSION_1_1
//	// OpenGL-1.1 �ξ��ϥƥ�����������ϥХ������˹Ԥ�
//	SetTexture(texBlendFunc) ;
//#endif

	SetTextureImage(image) ;
}

// ppm ����å����ɤ߹���
// �����ɼ��Ի��� NULL
TRUEIMAGE *LoadCacheImage(String& name)
{
	FILE *fp ;
	if (!(fp = fopen(name, "rb")))
		return NULL ;

	TRUEIMAGE *image = NULL ;
	image = finputpnm(fp) ;

	fclose(fp) ;
	return image ;
}


TRUEIMAGE *LoadTextueImage(String name)
{
	TRUEIMAGE *image = NULL ;

	FILE   *fp = fopen(name, "rb") ;
	if (fp)
	{
		image = finputpnm(fp) ;
		fclose(fp) ;
	}

	return image ;
}


// pnm �������ɤ߹���
void LoadTextureImage(TRUEIMAGE **image, String name, int nPath, String *path)
{
#ifdef WIN32
	char slash = '\\' ;
#else
	char slash = '/' ;
#endif

	String imagePath ;
	String ext = Lower(Right(name, 4)) ;

	if (ext != ".ppm" && ext != ".pgm" && ext != ".pbm")
		ext = "" ;

	for (int i = 0 ; i < nPath ; i ++)
	{
		if (path[i] == "" || Right(path[i], 1) == slash)
			imagePath = path[i] + name ;
		else
			imagePath = path[i] + slash + name ;

		String tmp = imagePath ;

		// ppm
		if (ext != ".pgm" && ext!= ".pbm")
		{
			if (ext != ".ppm")
				imagePath += ".ppm" ;
			*image = LoadTextueImage(imagePath) ;
		}
		if (*image) break ;

		// pgm
		imagePath = tmp ;
		if (ext != ".ppm" && ext!= ".pbm")
		{
			if (ext != ".pgm")
				imagePath += ".pgm" ;
			*image = LoadTextueImage(imagePath) ;
		}
		if (*image) break ;

		// pbm
		imagePath = tmp ;
		if (ext != ".ppm" && ext!= ".pgm")
		{
			if (ext != ".pbm")
				imagePath += ".pbm" ;
			*image = LoadTextueImage(imagePath) ;
		}

		if (*image) break ;
	}

	if (!(*image))
	{
#ifdef WIN32
		if (!saver)
			MessageBox(NULL, _T(String("Can't open file \"") + name + "\" in path."), _T("Loading Image Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf(String("Can't open file \"") + name + "\" in path.\n") ;

		logprintf("\tUse null image as \"" + name + "\".\n") ;
		*image = NewDefaultImage(defaultTextureSize) ;
	}
	else
		logprintf("Loading ... \"" + imagePath + "\".\n") ;
}

void LoadTextureImage(TRUEIMAGE **image, String& name)
{
	String path[3] ;

	path[0] = image_path ;	// -ip
	path[1] = "" ;			// current
	path[2] = env_path ;	// env "GLCLOCK_IMAGE_PATH"

	LoadTextureImage(image, name, 3, path) ;
}

/*
void LoadTextureImage(TRUEIMAGE **image, String& name)
{
	FILE   *fp ;
	char   *e_path ;
	String env_path ;
	int    flagOK = FALSE ;
	int    path_failed = FALSE ;

	if (image_path != "")
	{
		if (fp = fopen(image_path + name, "rb"))
		{
			env_path = image_path ;
			flagOK = TRUE ;
		}
	}

	if (!flagOK)
	{
		if (!(fp = fopen(name, "rb")))
		{
			if (!(e_path = getenv(ENV_IMAGE_PATH)))
			{
#ifdef WIN32
				if (!saver)
					MessageBox(NULL, _T(String("Can't open file \"") + name + "\"."), _T("Loading Image Error"), MB_OK | MB_ICONSTOP) ;
#endif
				errprintf(String("Can't open file \"") + name + "\".\n") ;

				path_failed = TRUE ;
//				EndProc() ;
//				glclock_exit(EXIT_FAILURE) ;
			}
			else
			{
				env_path = e_path ;
				env_path.Cut() ;

#ifdef WIN32
				if (Right(env_path) != '\\') env_path += '\\' ;
#else
				if (Right(env_path) != '/') env_path += '/' ;
#endif

				if (!(fp = fopen(env_path + name, "rb")))
				{
#ifdef WIN32
					if (!saver)
						MessageBox(NULL, _T(String("Can't open file \"") + name + "\"."), _T("Loading Image Error"), MB_OK | MB_ICONSTOP) ;
#endif
					errprintf(String("Can't open file \"") + name + "\".\n") ;

					path_failed = TRUE ;
//					EndProc() ;
//					glclock_exit(EXIT_FAILURE) ;
				}
			}
		}
	}

	if (!path_failed)
	{
		if (!(*image = finputpnm(fp)))
		{
			fclose(fp) ;

#ifdef WIN32
			if (!saver)
				MessageBox(NULL, _T(String("\"") + env_path + name + "\" isn't ppm or pgm format."), _T("Image Format Error"), MB_OK | MB_ICONSTOP) ;
#endif
			errprintf((String("\"") + env_path) + name + "\" isn't ppm or pgm format.\n") ;

			path_failed = TRUE ;
			EndProc() ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}

	if (path_failed)
	{
		logprintf("Use null image.\n") ;
		*image = NewDefaultImage(defaultTextureSize) ;
	}

	if (!path_failed)
		fclose(fp) ;
}
*/

// ���᡼���� ���� n�� �������˥�������󥰡ʺ��祵������ۤ�����Ϻ��祵�����ˡ�
void ScaleTextureImage(TRUEIMAGE ** image, int n = 0)
{
	GLint max_size, width, height, err ;
	TRUEIMAGE *s_image ;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size) ;	// �ƥ�������κ��祵����

	width  = (*image)->width ;
	height = (*image)->height ;

	if (width  > max_size) width  = max_size ;
	if (height > max_size) height = max_size ;

	if (width == (*image)->width && height == (*image)->height && n == 0) return ;

	if (n > 0) width = height = Pow(2, n) ;
	if (width > max_size || n < 0) width = height = max_size ;

	if (!(s_image = (TRUEIMAGE*)malloc(sizeof(TRUEIMAGE))))
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Cant't allocate memory."), _T("glclock error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't allocate memory.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}

	if (!(s_image->dataptr = (unsigned char *)malloc(width * height * 3)))
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Cant't allocate memory."), _T("glclock error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't allocate memory.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}

	logprintf("Scaling texture image.\n") ;

	if (err = gluScaleImage(GL_RGB, (*image)->width, (*image)->height, GL_UNSIGNED_BYTE, (*image)->dataptr, width,height, GL_UNSIGNED_BYTE, s_image->dataptr))
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		FreeImage(&s_image) ;
		glclock_exit(EXIT_FAILURE) ;
	}

	logprintf("Enable %dx%d image.\n", width, height) ;

	s_image->width  = width ;
	s_image->height = height ;

	freeimage(*image) ;
	*image = s_image ;
}

// ����Υ��᡼�����顢������ʬ�䤷������Υ��᡼�������
TRUEIMAGE *CreateImageFrom2Images(TRUEIMAGE* solid_image, TRUEIMAGE* env_image)
{
	GLint     max_size, image_size, width, height, pixels, err ;
	unsigned  char *s_image, *e_image ;
	TRUEIMAGE *image ;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size) ;	// �ƥ�������κ��祵����

	// �Ǥ��礭���������˹�碌��
	image_size = solid_image->width ;
	image_size = (solid_image->height > image_size ? solid_image->height : image_size) ;
	image_size = (env_image->width > image_size ? env_image->width : image_size) ;
	image_size = (env_image->height > image_size ? env_image->height : image_size) ;

/*
	if (image_size * 2 > max_size) return (TRUEIMAGE *)NULL ;
	else
	{
		for (width = 64 ; width < image_size ; width *= 2) ;
		height = width ;
	}
	pixels = width * height ;
*/
//
	for (width = 64 ; width < image_size ; width *= 2) ;
	height = width ;
	pixels = width * height ;
//

	if (!(s_image = (unsigned char*)malloc(pixels * 3))) return (TRUEIMAGE *)NULL ;
	if (!(e_image = (unsigned char*)malloc(pixels * 3))) return (TRUEIMAGE *)NULL ;

	err = gluScaleImage(GL_RGB, solid_image->width, solid_image->height, GL_UNSIGNED_BYTE, solid_image->dataptr, width,height, GL_UNSIGNED_BYTE, s_image) ;
	if (err)
	{
		free(s_image) ;
		free(e_image) ;
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		glclock_exit(EXIT_FAILURE) ;
	}

	err = gluScaleImage(GL_RGB, env_image->width, env_image->height, GL_UNSIGNED_BYTE, env_image->dataptr, width,height, GL_UNSIGNED_BYTE, e_image) ;
	if (err)
	{
		free(s_image) ;
		free(e_image) ;
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		glclock_exit(EXIT_FAILURE) ;
	}

	if (!(image = (TRUEIMAGE*)malloc(sizeof(TRUEIMAGE)))) return (TRUEIMAGE *)NULL ;

	image->width  = width * 2 ;
	image->height = height ;

	if (!(image->dataptr = (unsigned char *)malloc(pixels * 2 * 3))) return (TRUEIMAGE *)NULL ;

	// ���᡼���ΤĤʤ���碌
	for (int y = 0 ; y < height ; y ++)
	{
		int x ;

		// ����åɥ��᡼����������饹���˥ǡ������ϡʺ����
		for (x = 0 ; x < width ; x ++)
		{
			image->dataptr[y * width * 2 * 3 + x * 3] = s_image[y * width * 3 + x * 3] ;
			image->dataptr[y * width * 2 * 3 + x * 3 + 1] = s_image[y * width * 3 + x * 3 + 1] ;
			image->dataptr[y * width * 2 * 3 + x * 3 + 2] = s_image[y * width * 3 + x * 3 + 2] ;

		}

		// �Ķ����᡼���������饹���˥ǡ������ϡʱ����
		for (x = 0 ; x < width ; x ++)
		{
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3] = e_image[y * width * 3 + x * 3] ;
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3 + 1] = e_image[y * width * 3 + x * 3+ 1] ;
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3 + 2] = e_image[y * width * 3 + x * 3 + 2] ;

		}
	}

	free(s_image) ;
	free(e_image) ;

	return image ;
}

// ����ʥ���åɡ��Ķ�������åɣ��ˤΥ��᡼�����顢��������Ӿ岼��ʬ�䤷������Υ��᡼�������
TRUEIMAGE *CreateImageFrom2Images2(TRUEIMAGE* solid_image, TRUEIMAGE* env_image, TRUEIMAGE* solid2_image = NULL)
{
	GLint     max_size, image_size, width, height, pixels, err ;
	unsigned  char *s_image, *e_image, *s2_image ;
	TRUEIMAGE *image ;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size) ;	// �ƥ�������κ��祵����

	// �Ǥ��礭���������˹�碌��
	image_size = solid_image->width ;
	image_size = (solid_image->height > image_size ? solid_image->height : image_size) ;
	image_size = (env_image->width > image_size ? env_image->width : image_size) ;
	image_size = (env_image->height > image_size ? env_image->height : image_size) ;
	if (solid2_image)
	{
		image_size = (solid2_image->width > image_size ? solid2_image->width : image_size) ;
		image_size = (solid2_image->height > image_size ? solid2_image->height : image_size) ;
	}
/*
	if (image_size * 2 > max_size) return (TRUEIMAGE *)NULL ;
	else
	{
		for (width = 64 ; width < image_size ; width *= 2) ;
		height = width ;
	}
	pixels = width * height ;
*/
//
	for (width = 64 ; width < image_size ; width *= 2) ;
	height = width ;
	pixels = width * height ;
//

	if (!(s_image = (unsigned char*)malloc(pixels * 3))) return (TRUEIMAGE *)NULL ;
	if (!(e_image = (unsigned char*)malloc(pixels * 3))) return (TRUEIMAGE *)NULL ;
	if (solid2_image)
		if (!(s2_image = (unsigned char*)malloc(pixels * 3))) return (TRUEIMAGE *)NULL ;

	err = gluScaleImage(GL_RGB, solid_image->width, solid_image->height, GL_UNSIGNED_BYTE, solid_image->dataptr, width,height, GL_UNSIGNED_BYTE, s_image) ;
	if (err)
	{
		free(s_image) ;
		free(e_image) ;
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		glclock_exit(EXIT_FAILURE) ;
	}

	err = gluScaleImage(GL_RGB, env_image->width, env_image->height, GL_UNSIGNED_BYTE, env_image->dataptr, width,height, GL_UNSIGNED_BYTE, e_image) ;
	if (err)
	{
#ifdef WIN32
		MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
		free(s_image) ;
		free(e_image) ;
		EndProc() ;
		glclock_exit(EXIT_FAILURE) ;
	}

	if (solid2_image)
	{
		err = gluScaleImage(GL_RGB, solid2_image->width, solid2_image->height, GL_UNSIGNED_BYTE, solid2_image->dataptr, width,height, GL_UNSIGNED_BYTE, s2_image) ;
		if (err)
		{
			free(s_image) ;
			free(e_image) ;
			free(s2_image) ;
			EndProc() ;
#ifdef WIN32
			MessageBox(NULL, _T((char *)gluErrorString((GLenum)err)), _T("GLU Error"), MB_OK | MB_ICONSTOP) ;
#endif
			errprintf("GLU Error: %s\n", (char *)gluErrorString((GLenum)err)) ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}
	else
		s2_image = s_image ;

	if (!(image = (TRUEIMAGE*)malloc(sizeof(TRUEIMAGE)))) return (TRUEIMAGE *)NULL ;

	image->width  = width  * 2 ;
	image->height = height * 2;

	if (!(image->dataptr = (unsigned char *)malloc(pixels * 2 * 2 * 3))) return (TRUEIMAGE *)NULL ;

	// ���᡼���ΤĤʤ���碌
	for (int y = 0 ; y < height ; y ++)
	{
		int x ;

		// ����åɥ��᡼����������饹���˥ǡ�������
		for (x = 0 ; x < width ; x ++)
		{
			image->dataptr[y * width * 2 * 3 + x * 3] = s_image[y * width * 3 + x * 3] ;
			image->dataptr[y * width * 2 * 3 + x * 3 + 1] = s_image[y * width * 3 + x * 3 + 1] ;
			image->dataptr[y * width * 2 * 3 + x * 3 + 2] = s_image[y * width * 3 + x * 3 + 2] ;

		}

		// ����åɥ��᡼����������饹���˥ǡ������ϡʺ�����
		for (x = 0 ; x < width ; x ++)
		{
			image->dataptr[(y + height) * width * 2 * 3 + x * 3] = s2_image[y * width * 3 + x * 3] ;
			image->dataptr[(y + height) * width * 2 * 3 + x * 3 + 1] = s2_image[y * width * 3 + x * 3 + 1] ;
			image->dataptr[(y + height) * width * 2 * 3 + x * 3 + 2] = s2_image[y * width * 3 + x * 3 + 2] ;

		}

		// �Ķ����᡼���������饹���˥ǡ�������
		for (x = 0 ; x < width ; x ++)
		{
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3] = e_image[y * width * 3 + x * 3] ;
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3 + 1] = e_image[y * width * 3 + x * 3+ 1] ;
			image->dataptr[y * width * 2 * 3 + width * 3 + x * 3 + 2] = e_image[y * width * 3 + x * 3 + 2] ;

			// �Ķ����᡼���������饹���˥ǡ������ϡʱ�����
			image->dataptr[(y + height) * width * 2 * 3 + width * 3 + x * 3] = e_image[y * width * 3 + x * 3] ;
			image->dataptr[(y + height) * width * 2 * 3 + width * 3 + x * 3 + 1] = e_image[y * width * 3 + x * 3+ 1] ;
			image->dataptr[(y + height) * width * 2 * 3 + width * 3 + x * 3 + 2] = e_image[y * width * 3 + x * 3 + 2] ;

		}
	}

	free(s_image) ;
	free(e_image) ;
	if (solid2_image)
		free(s2_image) ;

	return image ;
}


// ���᡼���ե�����͡���Υ��ե��å�������ӥѥ�̾����
// �������Ĵ�٤ơ�'.' ����� '\' �⤷���� '/' �ǤϤ��ޤ줿�ϰ�
String DeleteSuffix(const String& file)
{
#ifdef WIN32
	char dpath = '\\' ;
#else
	char dpath = '/' ;
#endif

	int start, end ;
	for (end = file.Length() ; end >= 1 ; end --)
	{
		if (file.Character(end) == '.')
			break ;
	}

	if (end <= 1)
		return file ;

	for (start = end - 1 ; start >= 1 ; start --)
	{
		if (file.Character(start) == dpath)
			break ;
	}

	String nfile = Mid(file, start + 1, end - start - 1) ;
	return nfile ;
}


int SaveImage(const String& file, TRUEIMAGE *image, const String& comment = "Generated by glclock PPM Image")
{
	FILE *fp ;
	fp = fopen(file, "wb") ;
	if (fp)
		outputppm(image, fp, comment) ;
	else
		return FALSE ;	// ��¸����

	fclose(fp) ;
	return TRUE ;	// ��¸����
}


// Test
void SetTexGenModeForVOODOO()
{
	if (envOpenGL != VOODOO_3DFX_ICD)
		return ;

	GLenum texGenMode = GL_SPHERE_MAP ;
	if (fakeSphereMap)
		texGenMode = GL_NORMAL_MAP ;	// GL_NORMAL_MAP_{ NV | EXT }

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
//	glEnable(GL_TEXTURE_GEN_S) ;
//	glEnable(GL_TEXTURE_GEN_T) ;
}


void AdjustImageSize(TRUEIMAGE **img1, TRUEIMAGE **img2)
{
	if ((*img1)->width == (*img2)->width && (*img1)->height == (*img2)->height)
		return ;

	unsigned int width  = Max((*img1)->width,  (*img2)->width) ;
	unsigned int height = Max((*img1)->height, (*img2)->height) ;

	TRUEIMAGE *image1 = ScaledNewImage(*img1, width, height) ;
	TRUEIMAGE *image2 = ScaledNewImage(*img2, width, height) ;

	FreeImage(img1) ;
	FreeImage(img2) ;
	*img1 = image1 ;
	*img2 = image2 ;
}


void AddLightToMetal(TRUEIMAGE *metal, TRUEIMAGE *light, float brightGamma = 0.7f, float ratio = 0.4f)
{
	TRUEIMAGE *specular_light = CopyImage(light) ;
	ChangeGammaBrightness(specular_light, brightGamma) ;
	AddImage(metal, specular_light, 1.0f, ratio) ;
	FreeImage(&specular_light) ;
}


void MakeTextureLists_solid()
{
	LoadTextureImage(&solid_image, solid_file) ;
	logprintf("Setting solid texture from \"" + solid_file + "\".\n") ;

	if (solid_saturation != SOLID_SATURATION)
		ChangeSaturation(solid_image, solid_saturation) ;
	if (solid_contrast != SOLID_CONTRAST)
		ChangeContrast(solid_image, solid_contrast) ;

	ScaleTextureImage(&solid_image, tex_size) ;
	TextureProc(solid_image) ;
	glEnable(GL_TEXTURE_2D) ;

	// ����åɥƥ��������Ѥ˹���򥻥å�
	glMatrixMode(GL_TEXTURE) ;
	glLoadIdentity() ;

	glTranslatef(0.5f, 0.5f, 0.0f) ;
	if (tex_rotate != 0.0f)
		glRotatef(tex_rotate, 0.0f,0.0f,1.0f) ;
//	glScalef(1.0f / s_scale, 1.0f / t_scale, 1.0f) ;

//	glEnable(GL_TEXTURE_GEN_S) ;
//	glEnable(GL_TEXTURE_GEN_T) ;
}


void MakeTextureLists_env()
{
		LoadTextureImage(&env_image, env_file) ;
		LoadTextureImage(&light_image, light_file) ;
		AdjustImageSize(&env_image, &light_image) ;
		ChangeSaturation(light_image, light_vividness) ;

		if (has_texture_object)
		{
			// Texture Object ��

			float	detail_bright = 1.0f ;

			// 2000.06.15 -e 1 ���ǥ��ƥ�����ƥ������㥵�ݡ���
			if (detail)
			{
				LoadTextureImage(&detail_image, detail_file) ;

				logprintf("Setting detail texture from \"" + detail_file + "\".\n") ;
				GammaCollection(detail_image, detailMapGamma) ;
				ScaleTextureImage(&detail_image, tex_size) ;

				TRUEIMAGE *bright = ScaledNewImage(detail_image, 1, 1) ;
				if (bright)
				{
					detail_texture_bright = (float)bright->dataptr[0] * GRAY_RATIO_R +
											(float)bright->dataptr[1] * GRAY_RATIO_G +
											(float)bright->dataptr[2] * GRAY_RATIO_B ;
					FreeImage(&bright) ;

					if (detail_texture_bright <= 0.0f || detail_texture_bright > 255.0f)
						detail_texture_bright = 255.0f ;

					logprintf("\tAverage brightness: %3.1f\n", detail_texture_bright) ;
					if (detail_texture_bright > 0.0f)
						detail_texture_correct_bright = 255.0f / detail_texture_bright ;

					// -env �ˤϥ���åɥƥ��������¸�ߤ��ʤ����ᡢ
					// �ǥ��ƥ�����ƥ���������ε���������
					// DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE �˴ؤ�餺 Diffuse �Τߤ�����

					// �ǥ��ƥ�����ƥ�������͡������
					AllocATexture(DETAIL_TEXTURE_NAME) ;
					// �ǥ��ƥ�����ƥ��������Х���ɤ����Ķ�����
					glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
					TextureProc(detail_image) ;

					if (mode_env == 1)
						detail_bright = detail_texture_correct_bright ;
				}
			}


			// �Ķ��ޥåײ����Υ��ԡ�����
			specularImage = CreateCopyImage(env_image) ;
			if (!specularImage)
			{
				EndProc() ;
#ifdef WIN32
				MessageBox(NULL, _T("Can't Create Copy Image."), _T("Createing Image error"), MB_OK | MB_ICONSTOP) ;
#endif
				errprintf("Can't Create Copy Image.\n") ;
				glclock_exit(EXIT_FAILURE) ;
			}

			if (mode_env == 1 || mode_env == 2)
			{
				// -e 1, 2
				logprintf("Setting specular texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;

				ChangeGammaBrightness(specularImage, SPECULAR_MAP_GAMMA, RADIUS_RATIO_SPECULAR) ;
				AddImage(specularImage, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO) ;
				ScaleTextureImage(&specularImage, tex_size) ;

				logprintf("Setting chrome texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;

				if (mode_env == 1)
				{
					// -env 1
					ChangeSaturation(env_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME) ;
					ChangeBrightness(env_image, CHROME_BRIGHT, RADIUS_RATIO_CHROME) ;
					AddImage(env_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;
				}
				else
				{
					// -env 2
					env2_image = CreateCopyImage(env_image) ;

					// env_image �������ǥ��󥰤ʤ���°
					ChangeSaturation(env_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME) ;
					ChangeBrightness(env_image, CHROME_BRIGHT, RADIUS_RATIO_CHROME) ;
					AddImage(env_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;

					// env2_image �������ǥ����դ���°
					ChangeSaturation(env2_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME) ;
					ChangeBrightness(env2_image, CHROME_ENV2_ENV_BRIGHT, RADIUS_RATIO_CHROME) ;
					AddImage(env2_image, light_image, 1.0f, 1.0f) ;
				}

				ScaleTextureImage(&env_image, tex_size) ;
			}
			else if (!crystal)
			{
				// -e 3
				logprintf("Setting specular texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeGammaBrightness(specularImage, SPECULAR_MAP_GAMMA, RADIUS_RATIO_SPECULAR) ;
				AddImage(specularImage, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO) ;
				ScaleTextureImage(&specularImage, tex_size) ;

				logprintf("Setting chrome texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeSaturation(env_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME) ;
				ChangeBrightness(env_image, CHROME_BRIGHT, RADIUS_RATIO_CHROME) ;
				AddImage(env_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;
				ScaleTextureImage(&env_image, tex_size) ;
			}
			else
			{
				// ���ꥹ������ -C
				logprintf("Setting specular textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;

				ChangeGammaBrightness(light_image, CRYSTAL_LIGHT_MAP_GAMMA, RADIUS_RATIO_CRYSTAL) ;	// ���ꥹ����ϥ饤�ȥޥåפ�����

				ChangeGammaBrightness(specularImage, CRYSTAL_SPECULAR_MAP_GAMMA, RADIUS_RATIO_CRYSTAL) ;
				AddImage(specularImage, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;
				ScaleTextureImage(&specularImage, tex_size) ;

				ChangeGammaBrightness(env_image, CRYSTAL_SPECULAR_MAP_GAMMA, RADIUS_RATIO_CRYSTAL) ;
				AddImage(env_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;
				ScaleTextureImage(&env_image, tex_size) ;
			}

			// ��°�ƥ�������͡������
			AllocATexture(CHROME_TEXTURE_NAME) ;
			// ��°�ƥ��������Х���ɤ����Ķ�����
			glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;

			if (detail_bright != 1.0f)	// -e 1 -D
			{
				metalic_image = CopyImage(env_image) ;
				ChangeBrightness(metalic_image, detail_bright) ;
				TextureProc(metalic_image) ;
				FreeImage(&metalic_image) ;
			}
			else
			{
				TextureProc(env_image) ;
			}

			// ���ڥ���顼�ƥ�������͡������
			AllocATexture(SPECULAR_TEXTURE_NAME) ;
			// ���ڥ���顼�ƥ��������Х���ɤ����Ķ�����
			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
			TextureProc(specularImage) ;


			// -e 1 -e 2
			// �ƶ�°�ο�������ѤΥƥ�����������
			if (mode_env == 1 || mode_env == 2)
			{
				logprintf("Setting colored chrome textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;

				// �Ķ��ޥåײ����Υ��ԡ�����
				if (mode_env == 1)
				{
					gold_image    = CopyImage(env_image) ;
					metalic_image = CopyImage(env_image) ;

					// �������ѥ᥿��å��ʤ����ǤϻȤ�ʤ��ˤ���ӥ�����ɰʳ��ο��դ���°�ƥ�������
					// green, blue, red �ϡ��������򣱣����ʲ��ˤ����-CONSTEX ������Τߡ�
					if (conserveTex && (env_image->width > 128 || env_image->height > 128))
						green_image = ScaledNewImage(env_image, 128, 128) ;
					else
						green_image = CopyImage(env_image) ;

					// env_image ���顢���ˤ�äƤϥ���������Ȥ������᡼�������
					blue_image  = CopyImage(green_image) ;
					red_image   = CopyImage(green_image) ;
				}
				else
				{
					// env 2
					gold_image    = CopyImage(env_image) ;

					metalic_image = CopyImage(env2_image) ;
					pink_image    = CopyImage(env2_image) ;
//					gold2_image   = CopyImage(env2_image) ;

					// �������ѥ᥿��å��ʤ����ǤϻȤ�ʤ��ˤ���ӥ�����ɰʳ��ο��դ���°�ƥ�������
					// green, blue, red �ϡ��������򣱣����ʲ��ˤ����-CONSTEX ������Τߡ�
					if (conserveTex && (env_image->width > 128 || env_image->height > 128))
						green_image = ScaledNewImage(env_image, 128, 128) ;
					else
						green_image = CopyImage(env_image) ;

					// env_image ���顢���ˤ�äƤϥ���������Ȥ������᡼�������
					blue_image  = CopyImage(green_image) ;
					red_image   = CopyImage(green_image) ;

/*
					ModulateRGB(gold2_image, COLOR_GOLD_TEXTURE_R * COLOR_GOLD_TEXTURE,
											 COLOR_GOLD_TEXTURE_G * COLOR_GOLD_TEXTURE,
											 COLOR_GOLD_TEXTURE_B * COLOR_GOLD_TEXTURE, RADIUS_RATIO_CHROME) ;	// �⿧����Ĵ
					// ����ƥ�������͡������
					AllocATexture(GOLD2_TEXTURE_NAME) ;
					// ����ƥ��������Х���ɤ����Ķ�����
					glBindTexture(GL_TEXTURE_2D, GOLD2_TEXTURE_NAME) ;
					TextureProc(gold2_image) ;
*/
				}

				// �������Ӻ�����OpenGL-1.1 �ʹߤΤߡ�
				ModulateRGB(gold_image, COLOR_GOLD_TEXTURE_R * COLOR_GOLD_TEXTURE,
										COLOR_GOLD_TEXTURE_G * COLOR_GOLD_TEXTURE,
										COLOR_GOLD_TEXTURE_B * COLOR_GOLD_TEXTURE, RADIUS_RATIO_CHROME) ;	// �⿧����Ĵ
				AddLightToMetal(gold_image, light_image) ;		// ����˥饤�Ȥ�û�
				// ����ƥ�������͡������
				AllocATexture(GOLD_TEXTURE_NAME) ;
				// ����ƥ��������Х���ɤ����Ķ�����
				glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
				ChangeBrightness(gold_image, detail_bright) ;
				TextureProc(gold_image) ;


				// �����뿧�ʾ������������ä���°�ˤΥƥ�����������Ӻ�����OpenGL-1.1 �ʹߤΤߡ�
				ModulateRGB(metalic_image, COLOR_CHROME2_TEXTURE_R * COLOR_CHROME2_TEXTURE,
										   COLOR_CHROME2_TEXTURE_G * COLOR_CHROME2_TEXTURE,
										   COLOR_CHROME2_TEXTURE_B * COLOR_CHROME2_TEXTURE, RADIUS_RATIO_CHROME) ;	// ��°�����Ѵ�
				AddLightToMetal(metalic_image, light_image, 0.3, 0.3) ;		// ����˥饤�Ȥ�û�
				// ������ƥ�������͡������
				AllocATexture(CHROME2_TEXTURE_NAME) ;
				// �����뿧�ƥ��������Х���ɤ����Ķ�����
				glBindTexture(GL_TEXTURE_2D, CHROME2_TEXTURE_NAME) ;
				ChangeBrightness(metalic_image, detail_bright) ;
				TextureProc(metalic_image) ;


				// ���꡼���°������OpenGL-1.1 �ʹߤΤߡ�
				ModulateRGB(green_image, COLOR_GREEN_TEXTURE_R * COLOR_GREEN_TEXTURE,
										 COLOR_GREEN_TEXTURE_G * COLOR_GREEN_TEXTURE,
										 COLOR_GREEN_TEXTURE_B * COLOR_GREEN_TEXTURE, RADIUS_RATIO_CHROME) ;	// �п�����Ĵ
				// ���꡼���°�ƥ�������͡������
				AllocATexture(GREEN_TEXTURE_NAME) ;
				// ���꡼���°�ƥ��������Х���ɤ����Ķ�����
				glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
				TextureProc(green_image) ;

				// �֥롼��°������OpenGL-1.1 �ʹߤΤߡ�
				ModulateRGB(blue_image, COLOR_BLUE_TEXTURE_R * COLOR_BLUE_TEXTURE,
										COLOR_BLUE_TEXTURE_G * COLOR_BLUE_TEXTURE,
										COLOR_BLUE_TEXTURE_B * COLOR_BLUE_TEXTURE, RADIUS_RATIO_CHROME) ;	// �֥롼����Ĵ
				// �֥롼��°�ƥ�������͡������
				AllocATexture(BLUE_TEXTURE_NAME) ;
				// �֥롼��°�ƥ��������Х���ɤ����Ķ�����
				glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
				TextureProc(blue_image) ;

				// ��åɶ�°������OpenGL-1.1 �ʹߤΤߡ�
				ModulateRGB(red_image, COLOR_RED_TEXTURE_R * COLOR_RED_TEXTURE,
									   COLOR_RED_TEXTURE_G * COLOR_RED_TEXTURE,
									   COLOR_RED_TEXTURE_B * COLOR_RED_TEXTURE, RADIUS_RATIO_CHROME) ;	// ��åɤ���Ĵ
				// ��åɶ�°�ƥ�������͡������
				AllocATexture(RED_TEXTURE_NAME) ;
				// ��åɶ�°�ƥ��������Х���ɤ����Ķ�����
				glBindTexture(GL_TEXTURE_2D, RED_TEXTURE_NAME) ;
				TextureProc(red_image) ;


				// �ԥ󥯶�°������-env 2 �Τߡ�
				if (mode_env == 2)
				{
					ModulateRGB(pink_image, COLOR_PINK_TEXTURE_R * COLOR_PINK_TEXTURE,
											COLOR_PINK_TEXTURE_G * COLOR_PINK_TEXTURE,
											COLOR_PINK_TEXTURE_B * COLOR_PINK_TEXTURE, RADIUS_RATIO_CHROME) ;	// �ԥ󥯤���Ĵ
					// �ԥ󥯶�°�ƥ�������͡������
					AllocATexture(PINK_TEXTURE_NAME) ;
					// �ԥ󥯶�°�ƥ��������Х���ɤ����Ķ�����
					glBindTexture(GL_TEXTURE_2D, PINK_TEXTURE_NAME) ;
					TextureProc(pink_image) ;
				}
			}

			// �ǥե���Ȥ϶�°
			glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
			// ���ڥ���顼�ƥ��������ڤ��ؤ��ѥǥ����ץ쥤�ꥹ��
			glNewList(AllocAList(ENV_GLASS_TEXTURE_LIST), GL_COMPILE) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
			glEndList() ;

			FreeImage(&specularImage) ;

			if (!detail)
			{
				// �ǥե���Ⱦ��֤Υƥ���������������˥��å�
				glMatrixMode(GL_TEXTURE) ;
				glLoadIdentity() ;

				if (fakeSphereMap)	// �ե������Ķ��ޥå�
				{
//					glScalef(0.5f, 0.5f, 0.5f) ;
//					glTranslatef(1.0f, 1.0f, 1.0f) ;
					ScaleTranslateMatrixForFake() ;
				}
			}
			else
			{
#ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV

				// �ƥ������㥹�ơ������ʥ��ڥ���顼�Τߤ˻��ѡ�
				glMatrixMode(GL_TEXTURE) ;
				glLoadIdentity() ;

				if (fakeSphereMap)	// �ե������Ķ��ޥå�
				{
//					glScalef(0.5f, 0.5f, 0.5f) ;
//					glTranslatef(1.0f, 1.0f, 1.0f) ;
					ScaleTranslateMatrixForFake() ;
				}

				// GL_TEXTURE0 �μ�ư�����Ͼ�� ON
				glEnable(GL_TEXTURE_GEN_S) ;
				glEnable(GL_TEXTURE_GEN_T) ;

				// �ƥ������㥹�ơ������ʥǥ��ƥ�����ƥ�������Τߤ˻��ѡ�
				glActiveTexture(GL_TEXTURE1) ;
				glLoadIdentity() ;

				// GL_TEXTURE1 �μ�ư�����Ͼ�� OFF
				glDisable(GL_TEXTURE_GEN_S) ;
				glDisable(GL_TEXTURE_GEN_T) ;


#else	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV

				// �ƥ������㥹�ơ������ʥǥ��ƥ���������ڥ���顼�����˻��ѡ�
				glActiveTexture(GL_TEXTURE0) ;
				glMatrixMode(GL_TEXTURE) ;
				glLoadIdentity() ;

				// �ƥ������㥹�ơ������ʥ��ڥ���顼�Τߤ˻��ѡ�
				glActiveTexture(GL_TEXTURE1) ;

				// �ǥե���Ⱦ��֤Υƥ���������������˥��å�
				glMatrixMode(GL_TEXTURE) ;
				glLoadIdentity() ;

				if (fakeSphereMap)	// �ե������Ķ��ޥåס�GL_TEXTURE1 �ϴĶ��ޥåפˤΤ߻��ѡ�
				{
//					glScalef(0.5f, 0.5f, 0.5f) ;
//					glTranslatef(1.0f, 1.0f, 1.0f) ;
					ScaleTranslateMatrixForFake() ;
				}

				// GL_TEXTURE1 �μ�ư�����Ͼ�� ON
				glEnable(GL_TEXTURE_GEN_S) ;
				glEnable(GL_TEXTURE_GEN_T) ;

#endif	// #ifdef DETAIL_TEXTURE_BLEND_AFTER_ENV ... #else

				glActiveTexture(GL_TEXTURE0) ;
			}
		}
		else
		{
			// Texture Object �����Բ���
			if (!(create_image = CreateImageFrom2Images(env_image, env_image)))
			{
				EndProc() ;
#ifdef WIN32
				MessageBox(NULL, _T("Can't Create Texture Image."), _T("Createing Image error"), MB_OK | MB_ICONSTOP) ;
#endif
				errprintf("Can't Create Texture Image.\n") ;
				glclock_exit(EXIT_FAILURE) ;
			}
			FreeImage(&env_image) ;

			// ����ޥ��쥯�����������ѡ�
			if (env == 1)	// -e 1
			{
				logprintf("Setting specular sub texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeGammaBrightness(create_image, 0.5f, RADIUS_RATIO_SPECULAR, create_image->width / 2) ;	// ����ޥ��쥯�����ʥ��饹�ѡ�

				logprintf("Setting chrome sub texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeSaturation(create_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME,  0, 0, create_image->width / 2) ;

				AddImage(create_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO,  0.0f,  0, 0, create_image->width / 2) ;
				AddImage(create_image, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO,  0.0f,  create_image->width / 2) ;
			}
			else if (!crystal)	// -e 2, 3
			{
				logprintf("Setting specular sub textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeGammaBrightness(create_image, 0.5f, RADIUS_RATIO_SPECULAR, create_image->width / 2) ;	// ����ޥ��쥯�����ʥ��饹�ѡ�

				logprintf("Setting chrome sub texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
				ChangeSaturation(create_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME,  0, 0, create_image->width / 2) ;

				AddImage(create_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO,  0.0f,  0, 0, create_image->width / 2) ;
				AddImage(create_image, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO,  0.0f,  create_image->width / 2) ;
			}
			else	// -C
			{
				logprintf("Setting specular sub textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;

				ChangeGammaBrightness(create_image, CRYSTAL_SPECULAR_MAP_GAMMA, RADIUS_RATIO_CRYSTAL,  0, 0, create_image->width / 2) ;
				ChangeGammaBrightness(create_image, CRYSTAL_SPECULAR_MAP_GAMMA, RADIUS_RATIO_CRYSTAL, create_image->width / 2) ;	// ����ޥ��쥯�����ʥ��饹�ѡ�

				ChangeGammaBrightness(light_image, CRYSTAL_LIGHT_MAP_GAMMA, RADIUS_RATIO_CRYSTAL) ;		// ���ꥹ����ϥ饤�ȥޥåפ�����

				AddImage(create_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO,  0.0f,  0, 0, create_image->width / 2) ;
				AddImage(create_image, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO,  0.0f,  create_image->width / 2) ;
			}

			ScaleTextureImage(&create_image, tex_size) ;

			TextureProc(create_image) ;
			TextureListProc(GL_SPHERE_MAP, 0.0f,0.0f, 0.5f,1.0f) ;

			glNewList(AllocAList(ENV_GLASS_TEXTURE_LIST), GL_COMPILE) ;
				TextureCoordListProc(0.5f,0.0f, 0.5f,1.0f, fakeSphereMap) ;
//				TextureListProc(GL_SPHERE_MAP, 0.5f,0.0f, 0.5f,1.0f) ;
			glEndList() ;
		}

		// ���ä� enable �Τޤ�
		glEnable(GL_TEXTURE_2D) ;

		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;

}


void MakeTextureLists_real_texture_object(int solid2_flag)
{
	LoadTextureImage(&solid_image, solid_file) ;
	if (solid_saturation != SOLID_SATURATION)
		ChangeSaturation(solid_image, solid_saturation) ;
	if (solid_contrast != SOLID_CONTRAST)
		ChangeContrast(solid_image, solid_contrast) ;

	if (solid2_flag)
	{
		LoadTextureImage(&solid2_image, sld2_file) ;
		if (solid_saturation != SOLID_SATURATION)
			ChangeSaturation(solid2_image, solid_saturation) ;
		if (solid_contrast != SOLID_CONTRAST)
			ChangeContrast(solid2_image, solid_contrast) ;
	}

	if (detail)
		LoadTextureImage(&detail_image, detail_file) ;

	// ���ڥ���顼����°�ϥƥ��������
	// ����å��夬¸�ߤ�����ϥ���å��夫�������
	// ɬ�פʥ���å��夬���ĤǤ�¸�ߤ��ʤ����Ϥ��٤ƺ���
	String cache_file = "cache_glclock_" + DeleteSuffix(env_file) + "_" + DeleteSuffix(light_file) ;

	String cache_file_specular = cache_file + "_specular.ppm" ;
	String cache_file_chrome   = cache_file + "_chrome.ppm" ;
	String cache_file_gold     = cache_file + "_gold.ppm" ;
	String cache_file_green    = cache_file + "_green.ppm" ;
	String cache_file_blue     = cache_file + "_blue.ppm" ;
	String cache_file_red      = cache_file + "_red.ppm" ;
	String cache_file_multispecular = cache_file + "_multispecular.ppm" ;

	if (cacheImages)
	{
		specularImage = LoadCacheImage(cache_file_specular) ;
		env_image     = LoadCacheImage(cache_file_chrome) ;
		gold_image    = LoadCacheImage(cache_file_gold) ;
		green_image   = LoadCacheImage(cache_file_green) ;
		blue_image    = LoadCacheImage(cache_file_blue) ;
		red_image     = LoadCacheImage(cache_file_red) ;

//		if (reality >= 10)
		// �ޥ���ƥ���������ѻ��ǤϤʤ���ADD��Ȥ�ʤ������ѹ�
		if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)
			multiTexSpecularImage = LoadCacheImage(cache_file_multispecular) ;
	}

	// �����줫���ĤǤ�����ɤǤ��ʤ��ä����
//	if (!specularImage || !env_image || !gold_image || !green_image || !blue_image || !red_image ||
//		(reality >= 10 && !multiTexSpecularImage))
	if (!specularImage || !env_image || !gold_image || !green_image || !blue_image || !red_image ||
		(specularBlend == SPECULAR_BLEND_ADD_SMOOTH && !multiTexSpecularImage))
	{
		// ���٤��٤Ƴ���
		FreeImage(&specularImage) ;
		FreeImage(&env_image) ;
		FreeImage(&gold_image) ;
		FreeImage(&green_image) ;
		FreeImage(&blue_image) ;
		FreeImage(&red_image) ;
		FreeImage(&multiTexSpecularImage) ;

		LoadTextureImage(&env_image, env_file) ;
		LoadTextureImage(&light_image, light_file) ;
		AdjustImageSize(&env_image, &light_image) ;
		ChangeSaturation(light_image, light_vividness) ;

		ScaleTextureImage(&env_image, tex_size) ;

		// �Ķ��ޥåײ����Υ��ԡ�����
		specularImage = CopyImage(env_image) ;

		// ���ڥ���顼���᡼������
		logprintf("Setting specular textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
		ChangeGammaBrightness(specularImage, specularMapReflectGamma, RADIUS_RATIO_SPECULAR) ;	// ���٤Υ���ޥ��쥯�����ʥ�å��ѡ�
		AddImage(specularImage, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO) ;

		logprintf("Setting colored chrome textures from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
		// ��°�ƥ�������Ϻ��٤���Ȥ��Ƹ����û�
		ChangeSaturation(env_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME) ;
		ChangeBrightness(env_image, CHROME_BRIGHT, RADIUS_RATIO_CHROME) ;

		AddImage(env_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO) ;

		multiTexSpecularImage = CopyImage(specularImage) ;
		ChangeBrightness(multiTexSpecularImage, REALITY_SPECULAR_NA, RADIUS_RATIO_SPECULAR) ;	// ���ٸ���ʥޥ���ƥ��������å��ѡ�


		// ��°�ƥ������㤫�鿧�դ���°�ƥ����������
		gold_image = CopyImage(env_image) ;

		// �������ѥ᥿��å��ʤ����ǤϻȤ�ʤ��ˤ���ӥ�����ɰʳ��ο��դ���°�ƥ�������
		// green, blue, red �ϡ��������򣱣����ʲ��ˤ����-CONSTEX ������Τߡ�
		if (conserveTex && (env_image->width > 128 || env_image->height > 128))
			green_image = ScaledNewImage(env_image, 128, 128) ;
		else
			green_image = CopyImage(env_image) ;

		// env_image ���顢���ˤ�äƤϥ���������Ȥ������᡼�������
		blue_image  = CopyImage(green_image) ;
		red_image   = CopyImage(green_image) ;

		// �������Ӻ�����Texture Object ���ݡ��ȤΤߡ�
		ModulateRGB(gold_image, COLOR_GOLD_TEXTURE_R * COLOR_GOLD_TEXTURE,
								COLOR_GOLD_TEXTURE_G * COLOR_GOLD_TEXTURE,
								COLOR_GOLD_TEXTURE_B * COLOR_GOLD_TEXTURE, RADIUS_RATIO_CHROME) ;	// �⿧����Ĵ

		// ���꡼���°������OpenGL-1.1 �ʹߤΤߡ�
		ModulateRGB(green_image, COLOR_GREEN_TEXTURE_R * COLOR_GREEN_TEXTURE,
								 COLOR_GREEN_TEXTURE_G * COLOR_GREEN_TEXTURE,
								 COLOR_GREEN_TEXTURE_B * COLOR_GREEN_TEXTURE, RADIUS_RATIO_CHROME) ;	// ���꡼�����Ĵ

		// �֥롼��°������OpenGL-1.1 �ʹߤΤߡ�
		ModulateRGB(blue_image, COLOR_BLUE_TEXTURE_R * COLOR_BLUE_TEXTURE,
								COLOR_BLUE_TEXTURE_G * COLOR_BLUE_TEXTURE,
								COLOR_BLUE_TEXTURE_B * COLOR_BLUE_TEXTURE, RADIUS_RATIO_CHROME) ;	// �֥롼����Ĵ

		// ��åɶ�°������OpenGL-1.1 �ʹߤΤߡ�
		ModulateRGB(red_image, COLOR_RED_TEXTURE_R * COLOR_RED_TEXTURE,
							   COLOR_RED_TEXTURE_G * COLOR_RED_TEXTURE,
							   COLOR_RED_TEXTURE_B * COLOR_RED_TEXTURE, RADIUS_RATIO_CHROME) ;	// ��åɤ���Ĵ

		// ������������å�����¸
		if (cacheImages)
		{
			SaveImage(cache_file_specular, specularImage, "glclock Cache specular") ;
			SaveImage(cache_file_chrome,   env_image,     "glclock Cache Chrome") ;
			SaveImage(cache_file_gold,     gold_image,    "glclock Cache Gold") ;
			SaveImage(cache_file_green,    green_image,   "glclock Cache Green Chrome") ;
			SaveImage(cache_file_blue,     blue_image,    "glclock Cache Blue Chrome") ;
			SaveImage(cache_file_red,      red_image,     "glclock Cache Red Chrome") ;

//			if (reality >= 10)
			// �ޥ���ƥ���������ѻ��ǤϤʤ���ADD��Ȥ�ʤ������ѹ�
			if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)
				SaveImage(cache_file_multispecular, multiTexSpecularImage, "glclock Cache Specular for Multi-Texture") ;
		}

		// ����å��������λ
	}

	if (solid2_flag)
	{
		logprintf("Setting solid textures from \"" + solid_file + "\" and \"" + sld2_file + "\".\n") ;
		ScaleTextureImage(&solid2_image, tex_size) ;
	}
	else
	{
		logprintf("Setting solid texture from \"" + solid_file + "\".\n") ;
	}

	if (detail)
	{
//		LoadTextureImage(&detail_image, detail_file) ;
		logprintf("Setting detail texture from \"" + detail_file + "\".\n") ;
		GammaCollection(detail_image, detailMapGamma) ;
		ScaleTextureImage(&detail_image, tex_size) ;

		TRUEIMAGE *bright = ScaledNewImage(detail_image, 1, 1) ;
		if (bright)
		{
			detail_texture_bright = (float)bright->dataptr[0] * GRAY_RATIO_R +
									(float)bright->dataptr[1] * GRAY_RATIO_G +
									(float)bright->dataptr[2] * GRAY_RATIO_B ;
			FreeImage(&bright) ;

			if (detail_texture_bright <= 0.0f || detail_texture_bright > 255.0f)
				detail_texture_bright = 255.0f ;

			logprintf("\tAverage brightness: %3.1f\n", detail_texture_bright) ;
			if (detail_texture_bright > 0.0f)
				detail_texture_correct_bright = 255.0f / detail_texture_bright ;

#ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE

			// detail ������ϡ����餫���᥽��åɥƥ�������򾯤����뤯
			// ������ DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE �⡼�ɤǤ�
			// -r 1 �ξ��Τ�ͭ���ˡ�-r 2 �� Diffuse �ǽ�����
			if (mode_real == 1 || mode_real == 11)
#endif	// // #ifdef DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE
			{
				// detail ������ϡ����餫���᥽��åɥƥ�������򾯤����뤯
				ChangeBrightness(solid_image, detail_texture_correct_bright) ;
				if (solid2_flag)
					ChangeBrightness(solid2_image, detail_texture_correct_bright) ;
			}
		}
	}


	// ���ڥ���顼�ƥ�������͡������
	AllocATexture(SPECULAR_TEXTURE_NAME) ;
	// ���ڥ���顼�ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
	TextureProc(specularImage) ;

//	if (reality >= 10)
	// �ޥ���ƥ���������ѻ��ǤϤʤ���ADD��Ȥ�ʤ������ѹ�
	if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)
	{
		// �ޥ���ƥ�����������ڥ���顼�ƥ�������͡������
		AllocATexture(SPECULAR_MULTI_TEXTURE_NAME) ;
		// ���ڥ���顼�ƥ��������Х���ɤ����Ķ�����
		glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;
		// �ƥ�������ؿ��� GL_BLEND �Ȥʤ�
		TextureProc(multiTexSpecularImage) ;
	}
	else	// ADD ���ѻ��ϡ��̤Υ��ڥ���顼�ˤ⥰�饹�ƥ�����������
		SPECULAR_MULTI_TEXTURE_NAME = SPECULAR_TEXTURE_NAME ;


	// ��°�ƥ�������͡������
	AllocATexture(CHROME_TEXTURE_NAME) ;
	// ��°�ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
	TextureProc(env_image) ;


	// ����ƥ�������͡������
	AllocATexture(GOLD_TEXTURE_NAME) ;
	// ����ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
	AddLightToMetal(gold_image, light_image) ;		// ��Τߥ饤�Ȥ�û�
	TextureProc(gold_image) ;

	// ���꡼���°�ƥ�������͡������
	AllocATexture(GREEN_TEXTURE_NAME) ;
	// ���꡼���°�ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, GREEN_TEXTURE_NAME) ;
	TextureProc(green_image) ;

	// �֥롼��°�ƥ�������͡������
	AllocATexture(BLUE_TEXTURE_NAME) ;
	// �֥롼��°�ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, BLUE_TEXTURE_NAME) ;
	TextureProc(blue_image) ;

	// ��åɶ�°�ƥ�������͡������
	AllocATexture(RED_TEXTURE_NAME) ;
	// ��åɶ�°�ƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, RED_TEXTURE_NAME) ;
	TextureProc(red_image) ;

	ScaleTextureImage(&solid_image, tex_size) ;
	// ����åɣ��ƥ�������͡������
	AllocATexture(SOLID_TEXTURE_NAME) ;
	// ����åɥƥ��������Х���ɤ����Ķ�����
	glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
	TextureProc(solid_image) ;

	// ����åɣ��ƥ�������͡������
	if (solid2_flag)
	{
		AllocATexture(SOLID2_TEXTURE_NAME) ;
		// ����åɥƥ��������Х���ɤ����Ķ�����
		glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;
		TextureProc(solid2_image) ;
	}
	else
		SOLID2_TEXTURE_NAME = SOLID_TEXTURE_NAME ;

	if (detail)
	{
		// �ǥ��ƥ�����ƥ�������͡������
		AllocATexture(DETAIL_TEXTURE_NAME) ;
		// �ǥ��ƥ�����ƥ��������Х���ɤ����Ķ�����
		glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
		TextureProc(detail_image) ;
	}


	glNewList(AllocAList(SOLID_TEXTURE_LIST), GL_COMPILE) ;
		glBindTexture(GL_TEXTURE_2D, SOLID_TEXTURE_NAME) ;
		TextureListProc(GL_OBJECT_LINEAR) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;
	glEndList() ;

	if (detail)
	{
		glNewList(AllocAList(DETAIL_TEXTURE_LIST), GL_COMPILE) ;
			glBindTexture(GL_TEXTURE_2D, DETAIL_TEXTURE_NAME) ;
			TextureListProc(GL_OBJECT_LINEAR, 0.0f, 0.0f, 1.0f, 1.0f, FALSE) ;
			glDisable(GL_TEXTURE_GEN_S) ;
			glDisable(GL_TEXTURE_GEN_T) ;
			glMatrixMode(GL_MODELVIEW) ;
		glEndList() ;
	}

	glNewList(AllocAList(ENV_TEXTURE_LIST), GL_COMPILE) ;
		glBindTexture(GL_TEXTURE_2D, SPECULAR_TEXTURE_NAME) ;
		TextureListProc(GL_SPHERE_MAP) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;
	glEndList() ;


	// �ޥ���ƥ������㥹�ڥ���顼�ꥹ��
//	if (reality >= 10)
	if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)	// ONE_MINUS ���ѻ�
	{
		glNewList(AllocAList(SPECULAR_MULTI_TEXTURE_LIST), GL_COMPILE) ;
			glBindTexture(GL_TEXTURE_2D, SPECULAR_MULTI_TEXTURE_NAME) ;
			TextureListProc(GL_SPHERE_MAP) ;
			glEnable(GL_TEXTURE_GEN_S) ;
			glEnable(GL_TEXTURE_GEN_T) ;
			glMatrixMode(GL_MODELVIEW) ;
		glEndList() ;
	}

	glNewList(AllocAList(SOLID2_TEXTURE_LIST), GL_COMPILE) ;
	{
		if (solid2_flag)
		{
			glBindTexture(GL_TEXTURE_2D, SOLID2_TEXTURE_NAME) ;
		}
		TextureListProc(GL_OBJECT_LINEAR) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;
	}
	glEndList() ;

	glNewList(AllocAList(CHROME_TEXTURE_LIST), GL_COMPILE) ;
		glBindTexture(GL_TEXTURE_2D, CHROME_TEXTURE_NAME) ;
		TextureListProc(GL_SPHERE_MAP) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;
	glEndList() ;

	glNewList(AllocAList(GOLD_TEXTURE_LIST), GL_COMPILE) ;
		glBindTexture(GL_TEXTURE_2D, GOLD_TEXTURE_NAME) ;
		TextureListProc(GL_SPHERE_MAP) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;
	glEndList() ;

	FreeImage(&specularImage) ;
	if (multiTexSpecularImage)
		FreeImage(&multiTexSpecularImage) ;
}


void MakeTextureLists_real_non_texture_object(int solid2_flag)
{
	LoadTextureImage(&solid_image, solid_file) ;
	if (solid_saturation != SOLID_SATURATION)
		ChangeSaturation(solid_image, solid_saturation) ;
	if (solid_contrast != SOLID_CONTRAST)
		ChangeContrast(solid_image, solid_contrast) ;

	LoadTextureImage(&env_image, env_file) ;
	LoadTextureImage(&light_image, light_file) ;
	AdjustImageSize(&env_image, &light_image) ;
	ChangeSaturation(light_image, light_vividness) ;

	if (solid2_flag)
	{
		LoadTextureImage(&solid2_image, sld2_file) ;
		if (solid_saturation != SOLID_SATURATION)
			ChangeSaturation(solid2_image, solid_saturation) ;
		if (solid_contrast != SOLID_CONTRAST)
			ChangeContrast(solid2_image, solid_contrast) ;

		logprintf("Setting solid sub textures from \"" + solid_file + "\" and \"" + sld2_file + "\".\n") ;
	}
	else
		logprintf("Setting solid sub texture from \"" + solid_file + "\".\n") ;


	if (!(create_image = CreateImageFrom2Images2(solid_image, env_image, solid2_flag ? solid2_image : NULL)))
	{
		EndProc() ;
#ifdef WIN32
		MessageBox(NULL, _T("Can't Create Texture Image."), _T("Createing Image error"), MB_OK | MB_ICONSTOP) ;
#endif
		errprintf("Can't Create Texture Image.\n") ;
		glclock_exit(EXIT_FAILURE) ;
	}

	FreeImage(&solid_image) ;
	FreeImage(&env_image) ;
	FreeImage(&solid2_image) ;

	logprintf("Setting specular sub texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
//	GammaCollection(create_image, specularMapReflectGamma, 0.0f, create_image->width / 2, 0, create_image->width / 2, create_image->height / 2) ;	// ����ޥ��쥯�����ʥ�å��ѡ�
	ChangeGammaBrightness(create_image, specularMapReflectGamma, RADIUS_RATIO_SPECULAR, create_image->width / 2, 0, create_image->width / 2, create_image->height / 2) ;	// ����ޥ��쥯�����ʥ�å��ѡ�
	AddImage(create_image, light_image, 1.0f, SPECULAR_ADDING_LIGHT_RATIO, 0.0f, create_image->width / 2, 0, create_image->width / 2, create_image->height / 2) ;

	// ��°�ƥ�������ʱ����ˤϺ��٤���Ȥ��Ƹ����û�
	logprintf("Setting chrome sub texture from \"" + env_file + "\" and \"" + light_file + "\".\n") ;
	ChangeSaturation(create_image, CHROME_IMAGE_VIVIDNESS_RATIO, RADIUS_RATIO_CHROME, create_image->width / 2, create_image->height / 2, create_image->width / 2, create_image->height / 2) ;
//	ChangeGammaBrightness(create_image, CHROME_GAMMA_BRIGHT, RADIUS_RATIO_CHROME, create_image->width / 2, create_image->height / 2, create_image->width / 2, create_image->height / 2) ;
	AddImage(create_image, light_image, 1.0f, CHROME_ADDING_LIGHT_RATIO, 0.0f, create_image->width / 2, create_image->height / 2, create_image->width / 2, create_image->height / 2) ;

	ScaleTextureImage(&create_image, tex_size) ;
	TextureProc(create_image) ;

	glNewList(AllocAList(SOLID_TEXTURE_LIST), GL_COMPILE) ;
		TextureListProc(GL_OBJECT_LINEAR, 0.0f,0.0f, 0.5f,0.5f) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;
		glMatrixMode(GL_MODELVIEW) ;	// ����⡼�ɤ��ǥ�ӥ塼���᤹
	glEndList() ;

	glNewList(AllocAList(ENV_TEXTURE_LIST), GL_COMPILE) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;
		TextureListProc(GL_SPHERE_MAP, 0.5f,0.0f, 0.5f,0.5f) ;
		glMatrixMode(GL_MODELVIEW) ;	// ����⡼�ɤ��ǥ�ӥ塼���᤹
	glEndList() ;

	glNewList(AllocAList(SOLID2_TEXTURE_LIST), GL_COMPILE) ;
		TextureCoordListProc(0.0f,0.5f, 0.5f,0.5f) ;
		glDisable(GL_TEXTURE_GEN_S) ;
		glDisable(GL_TEXTURE_GEN_T) ;
		glTranslatef(0.5f, 0.5f, 0.0f) ;
//		glScalef(1.0f / s_scale, 1.0f / t_scale, 1.0f) ;
	glEndList() ;

	glNewList(AllocAList(CHROME_TEXTURE_LIST), GL_COMPILE) ;
		glEnable(GL_TEXTURE_GEN_S) ;
		glEnable(GL_TEXTURE_GEN_T) ;
		TextureCoordListProc(0.5f,0.5f, 0.5f,0.5f, fakeSphereMap) ;
	glEndList() ;
}


void MakeTextureLists_real()
{
	int solid2_flag = (sld2_file != String("")) ;

	if (has_texture_object)
	{
		// Texture Object ��
		MakeTextureLists_real_texture_object(solid2_flag) ;
	}
	else
	{
		// can't use Texture Object
		MakeTextureLists_real_non_texture_object(solid2_flag) ;
	}	// !has_texture_object
}


void MakeTextureLists()
{
	// ��ư�����⡼�ɤ�ɬ���Ķ��ޥå�
	GLenum texGenMode = GL_SPHERE_MAP ;
	if (fakeSphereMap)
		texGenMode = GL_NORMAL_MAP ;	// GL_NORMAL_MAP_[ NV | EXT ]

	if (reality >= 10)
	{
		if (detail == 3)
		{
			glActiveTexture(GL_TEXTURE2) ;
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
//			glEnable(GL_TEXTURE_GEN_S) ;
//			glEnable(GL_TEXTURE_GEN_T) ;
		}

		glActiveTexture(GL_TEXTURE0) ;
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glActiveTexture(GL_TEXTURE1) ;
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
	}
	else if (mode_env && detail)
	{
		glActiveTexture(GL_TEXTURE1) ;
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glActiveTexture(GL_TEXTURE0) ;
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
	}
	else
	{
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, texGenMode) ;
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, texGenMode) ;
	}


	if (solid && !reality)	// -s ���ץ�����
	{
		MakeTextureLists_solid() ;
	}
	else if (!reality)	// -e ���ץ�����
	{
		MakeTextureLists_env() ;
	}
	else	// -r ���ץ�����
	{
		MakeTextureLists_real() ;
	}

	FreeUsedImages() ;
}


// �ƥǥ����ץ쥤�ꥹ�Ⱥ���
void MakeDisplayLists()
{
#ifdef DEBUG_MATRIX_STACK
	// ���󥹥��å��ΥǥХå��⡼��
	extern GLint DEBUG_modelViewMatrixStack ;
	extern GLint DEBUG_projectionMatrixStack ;
	extern GLint DEBUG_textureMatrixStack ;
#endif

	if (texture)
		MakeTextureLists() ;	// �ƥ������������ѥꥹ�Ⱥ���

	MakeMaterialLists() ;		// ����ꥹ�Ⱥ���
	MakeClockNeedleLists() ;	// ���׿˥ꥹ��
	MakePolyhedronLists() ;		// ��¿���Υꥹ�Ⱥ���
	MakeShellLists() ;			// ���׳̥ꥹ�Ⱥ���
	MakeRoofLists() ;			// ���ꥹ��
	MakeBoardLists() ;			// ʸ���ץꥹ�Ⱥ���
	MakeGlassList() ;			// ɽ�̥��饹�ꥹ�Ⱥ���
	MakeSignList() ;			// �����Υ�����ꥹ�Ⱥ���
}

// ˾�󡢹��ѽ���
void SetPerspective(ClockState& st, GLfloat px = 0.0f, GLfloat py = 0.0f)
{
	float stpers = st.pers ;
	float stclock_z = st.clock_z ;
	if (!benchmark && mode_solid && (filterTest || mipmapTest))
	{
		stpers = 1.0f ;
		stclock_z = B_DIST ;
	}

	float pers = 1.0f / st.pers ;

	// ����åԥ󥰵�Υ�׻�
	far_clip = (near_clip = -stclock_z - 40.0f) + 80.0f ;
	if (near_clip < pers)
		if ((near_clip = pers) >= far_clip) far_clip = near_clip + 0.01f ;

	if (xsize > ysize) { scr_w = (GLfloat)xsize / ysize ; scr_h = 1.0f ; }
	else               { scr_h = (GLfloat)ysize / xsize ; scr_w = 1.0f ; }

	// �����꡼�󥵥������åȡʥ˥�������å��̤Υ�������
	scr_w *= stpers * near_clip * screen_size ;
	scr_h *= stpers * near_clip * screen_size ;

	Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, px,py) ;	// ����ԥ�ߥåɺ���
	glMatrixMode(GL_MODELVIEW) ;	// ����⡼�ɤ��ǥ�ӥ塼���᤹
}

/*
// �����󤪤�ӣƣУ�����
void DrawSignAndFPS()
{
	glPushAttrib(GL_ENABLE_BIT) ;

		glDisable(GL_LIGHTING) ;
		glDisable(GL_FOG) ;
		glDisable(GL_TEXTURE_2D) ;	// �ƥ�������ϣƣ�
		glDisable(GL_DEPTH_TEST) ;	// �ǥץ��ƥ��ȣϣƣơ�ɬ��ɽ����

		glPushMatrix() ;
		glLoadIdentity() ;	// ��ǥ�ӥ塼�����ñ�̹����

		glMatrixMode(GL_PROJECTION) ;
			glLoadIdentity() ;	// �ͱƹ����ñ�̹����

			if (!benchmark)
			{
				// �٥���ޡ����⡼�ɤǤϤʤ����
				glColor3f(currentCL[0]*0.7f, currentCL[0]*0.7f, currentCL[0]*0.7f) ;	// ���ߤΥ饤�ȿ��Σ���ε��٤�

				// ����������
				if (!no_sign) DrawString(sign, (sign_x - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
										 (sign_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
										 GLUT_BITMAP_TIMES_ROMAN_10) ;
				// �ƣУ�����
				if (f_prn) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
									  (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
									  GLUT_BITMAP_8_BY_13) ;

				// �ޥ���������������
				if (voodoo && (DragL || DragR)) DrawCharacter('+', ((mouse_x - 4) - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
															  ((ysize - (mouse_y + 5)) - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;

				glColor3fv(currentCL) ;		// ���ߤΥ饤�ȿ���

				// ����������
				if (!no_sign) DrawString(sign, (sign_x - xsize * 0.5f) / xsize * 2.0f,
										 (sign_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
										 GLUT_BITMAP_TIMES_ROMAN_10) ;
				// �ƣУ�����
				if (f_prn) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f,
									  (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
									  GLUT_BITMAP_8_BY_13) ;
				// �ޥ���������������
				if (voodoo && (DragL || DragR)) DrawCharacter('+', ((mouse_x - 4) - xsize * 0.5f) / xsize * 2.0f,
															  ((ysize - (mouse_y + 5)) - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;
			}
			else
			{
				// �٥���ޡ����⡼�ɤξ��ϣƣУӤΤ�ɽ��

				glColor3fv(currentCL) ;		// ���ߤΥ饤�ȿ���

				// �ƣУ�����
				if (f_prn || benchmark) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f,
												   (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
												   GLUT_BITMAP_8_BY_13) ;
			}

		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
		glPopMatrix() ;

	glPopAttrib() ;
}
*/

// �����󤪤�ӣƣУ�����
void DrawSignAndFPS()
{
	glPushAttrib(GL_ENABLE_BIT | GL_BLEND) ;
	{
		glDisable(GL_LIGHTING) ;
		if (fogFlag != FOG_NONE)
			glDisable(GL_FOG) ;

		glDisable(GL_TEXTURE_2D) ;	// �ƥ�������ϣƣ�
		glDisable(GL_DEPTH_TEST) ;	// �ǥץ��ƥ��ȣϣƣơ�ɬ��ɽ����

		if (!benchmark && envOpenGL != VOODOO_MESA_OLD && !mode_normal)
		{
			glEnable(GL_BLEND) ;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		}

//		glMatrixMode(GL_MODELVIEW) ;	// 2000.06.25 ���̤ʤ�
		glPushMatrix() ;
		glLoadIdentity() ;	// ��ǥ�ӥ塼�����ñ�̹����

		glMatrixMode(GL_PROJECTION) ;
			glPushMatrix() ;
			glLoadIdentity() ;	// �ͱƹ����ñ�̹����

			if (!benchmark)
			{
				// �٥���ޡ����⡼�ɤǤϤʤ����
				if (envOpenGL != VOODOO_MESA_OLD && !mode_normal)
					glColor4f(currentCL[0], currentCL[0], currentCL[0], 0.5f) ;
				else
					glColor3f(currentCL[0]*0.6f, currentCL[0]*0.6f, currentCL[0]*0.6f) ;	// ���ߤΥ饤�ȿ��Σ���ε��٤�

				// ����������
				if (!no_sign)
				{
					glRasterPos3f((sign_x - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
								  (sign_y - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;
					glCallList(SIGN_LIST) ;
				}

				// �ƣУ�����
				if (f_prn) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
									  (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
									  GLUT_BITMAP_8_BY_13) ;

				// �ޥ���������������
				if (drawCursor && (DragL || DragR))
					DrawCharacter('+', ((mouse_x - 4) - xsize * 0.5f) / xsize * 2.0f + 2.0f / xsize,
								  ((ysize - (mouse_y + 5)) - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;

				if (envOpenGL != VOODOO_MESA_OLD && !mode_normal)
					glColor4f(currentCL[0], currentCL[0], currentCL[0], 0.7f) ;
				else
					glColor3fv(currentCL) ;		// ���ߤΥ饤�ȿ���

				// ����������
				if (!no_sign)
				{
					glRasterPos3f((sign_x - xsize * 0.5f) / xsize * 2.0f,
								  (sign_y - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;
					glCallList(SIGN_LIST) ;
				}

				// �ƣУ�����
				if (f_prn) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f,
									  (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
									  GLUT_BITMAP_8_BY_13) ;
				// �ޥ���������������
				if (drawCursor && (DragL || DragR))
					DrawCharacter('+', ((mouse_x - 4) - xsize * 0.5f) / xsize * 2.0f,
								  ((ysize - (mouse_y + 5)) - ysize * 0.5f) / ysize * 2.0f, 0.0f) ;
			}
			else
			{
				// �٥���ޡ����⡼�ɤξ��ϣƣУӤΤ�ɽ��

				glColor3fv(currentCL) ;		// ���ߤΥ饤�ȿ���

				// �ƣУ�����
				if (f_prn || benchmark) DrawString(s_fps, (fps_x - xsize * 0.5f) / xsize * 2.0f,
												   (fps_y - ysize * 0.5f) / ysize * 2.0f, 0.0f,
												   GLUT_BITMAP_8_BY_13) ;
			}

		glPopMatrix() ;
		glMatrixMode(GL_MODELVIEW) ;
		glPopMatrix() ;

	}
	glPopAttrib() ;
}


// ���פθ����ˤ�륫��󥰥��ơ�����
// ����ˡ�����󥰤��θ��������ݥꥴ����� r_polygons �˥��å�
unsigned int CullingStatus(float zAxis)
{
	int status = CULL_STATUS_NONE ;

	if (cullObj)
		status |= CULL_STATUS_FRONT ;

	if (!noCullObj && !mode_crystal)
	{
		if (zAxis > CULL_SHELL_Z_AXIS)	// �������ѥ��������
			status |= CULL_STATUS_SHELL ;

		if (zAxis < CULL_BOARD_Z_AXIS)	// ����ɽ�̥��֥������Ȥ�����ʤ�
			status |= CULL_STATUS_FRONT ;
	}

	return status ;
}


// ��ǥ����衢���󥻥åȡ���ž�����ѡʥ⡼�����֥��ѡ�
// ����ϥ������졼�����Хåե���Ȥ�ʤ�������
void DrawModelMatrixWithOverSampling(ClockState p, ClockState& st, int count)
{
	// ������������ �Ȥꤢ���� count �ν�����ɤ�
	// �ǽ�Ū�ˤϡ�ü����������������ɤ���
	int ratio_no = count ;

	GLfloat f_r = 1.0f - shutter, r_step = shutter / (motion - 1), sp_x = spin_x, sp_y = spin_y, ratio ;

	ratio = f_r + r_step * ratio_no ;
	if (!ratio_no)
	{
		sp_x *= f_r ;
		sp_y *= f_r ;
	}
	else
	{
		sp_x *= r_step ;
		sp_y *= r_step ;
	}

	p.tm.hour   += (st.tm.hour   - p.tm.hour  ) * ratio ;
	p.tm.minute += (st.tm.minute - p.tm.minute) * ratio ;
	p.tm.second += (st.tm.second - p.tm.second) * ratio ;

	p.h_rotate  += (st.h_rotate  - p.h_rotate ) * ratio ;
	p.g_spin    += (st.g_spin    - p.g_spin   ) * ratio ;

	p.roof      += (st.roof      - p.roof     ) * ratio ;

	p.clock_z   += (st.clock_z   - p.clock_z  ) * ratio ;
	p.pers      += (st.pers      - p.pers     ) * ratio ;
	p.y_pers    += (st.y_pers    - p.y_pers   ) * ratio ;

	glMatrixMode(GL_PROJECTION) ;
	SetPerspective(p) ;

	glPushMatrix() ;
	{
		glPushMatrix() ;
		{
			glLoadIdentity() ;

/*
			glRotatef(sp_x, 0.0f,1.0f,0.0f) ;	// ������ž
			glRotatef(sp_y, 1.0f,0.0f,0.0f) ;	// ������ž
*/
			if (sp_x != 0.0f || sp_y != 0.0f)
			{
				static Vector3d axis ;

				axis(sp_y, sp_x, 0.0f) ;
				axis.Unit() ;
				glRotatef(sqrt(sp_x * sp_x + sp_y * sp_y), axis.X(), axis.Y(), axis.Z()) ;
			}

			glMultMatrixf(aRotate) ;
			glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
		}
		glPopMatrix() ;

		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, p.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -p.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		DrawClock(p, count) ;
	}
	glPopMatrix() ;
}

// ��ǥ����衢���󥻥åȡ���ž�����ѡʥ⡼�����֥��ѡ�
void DrawModelMatrix(ClockState p, ClockState& st, int count)
{
	GLfloat f_r = 1.0f - shutter, r_step = shutter / (motion - 1), sp_x = spin_x, sp_y = spin_y, ratio ;
/*
	GLfloat	sp_x = spin_x, sp_y = spin_y, ratio ;
	GLfloat f_r, r_step ;
	if (shutter == 0.0f)
	{
		f_r = 0.0f ;
		r_step = shutter / (motion - 1) ;
	}
	else
	{
		f_r = 1.0f / shutter ...
	}
*/

	ratio = f_r + r_step * count ;
	if (!count)
	{
		sp_x *= f_r ;
		sp_y *= f_r ;
	}
	else
	{
		sp_x *= r_step ;
		sp_y *= r_step ;
	}

	p.tm.hour   += (st.tm.hour   - p.tm.hour  ) * ratio ;
	p.tm.minute += (st.tm.minute - p.tm.minute) * ratio ;
	p.tm.second += (st.tm.second - p.tm.second) * ratio ;

	p.h_rotate  += (st.h_rotate  - p.h_rotate ) * ratio ;
	p.g_spin    += (st.g_spin    - p.g_spin   ) * ratio ;

	p.roof      += (st.roof      - p.roof     ) * ratio ;

	p.clock_z   += (st.clock_z   - p.clock_z  ) * ratio ;
	p.pers      += (st.pers      - p.pers     ) * ratio ;
	p.y_pers    += (st.y_pers    - p.y_pers   ) * ratio ;

	glMatrixMode(GL_PROJECTION) ;
	SetPerspective(p) ;

	glPushMatrix() ;
	{
		glPushMatrix() ;
		{
			glLoadIdentity() ;

/*
			glRotatef(sp_x, 0.0f,1.0f,0.0f) ;	// ������ž
			glRotatef(sp_y, 1.0f,0.0f,0.0f) ;	// ������ž
*/
			if (sp_x != 0.0f || sp_y != 0.0f)
			{
				static Vector3d axis ;

				axis(sp_y, sp_x, 0.0f) ;
				axis.Unit() ;
				glRotatef(sqrt(sp_x * sp_x + sp_y * sp_y), axis.X(), axis.Y(), axis.Z()) ;
			}

			glMultMatrixf(aRotate) ;
			glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
		}
		glPopMatrix() ;

		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, p.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -p.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		DrawClock(p) ;
	}
	glPopMatrix() ;
}

// ��ǥ����衢���󥻥åȡ���ž�����ѡʥ���ƥ������ꥢ���󥰡��⡼�����֥��ѡ�
void DrawModelMatrix(ClockState& p, ClockState& st, int count, ClockState *n)
{
	GLfloat f_r = 1.0f - shutter, r_step = shutter / (motion - 1), sp_x = spin_x, sp_y = spin_y, ratio ;

	ratio = f_r + r_step * count ;
	if (!count)
	{
		sp_x *= f_r ;
		sp_y *= f_r ;
	}
	else
	{
		sp_x *= r_step ;
		sp_y *= r_step ;
	}

	n->tm.hour   = p.tm.hour   + (st.tm.hour   - p.tm.hour  ) * ratio ;
	n->tm.minute = p.tm.minute + (st.tm.minute - p.tm.minute) * ratio ;
	n->tm.second = p.tm.second + (st.tm.second - p.tm.second) * ratio ;

	n->h_rotate  = p.h_rotate  + (st.h_rotate  - p.h_rotate ) * ratio ;
	n->g_spin    = p.g_spin    + (st.g_spin    - p.g_spin   ) * ratio ;

	n->roof      = p.roof      + (st.roof      - p.roof     ) * ratio ;

	n->clock_z   = p.clock_z   + (st.clock_z   - p.clock_z  ) * ratio ;
	n->pers      = p.pers      + (st.pers      - p.pers     ) * ratio ;
	n->y_pers    = p.y_pers    + (st.y_pers    - p.y_pers   ) * ratio ;

	glMatrixMode(GL_PROJECTION) ;
	SetPerspective(p) ;

	glPushMatrix() ;
	{
		glPushMatrix() ;
		{
			glLoadIdentity() ;

/*
			glRotatef(sp_x, 0.0f,1.0f,0.0f) ;	// ������ž
			glRotatef(sp_y, 1.0f,0.0f,0.0f) ;	// ������ž
*/
			if (sp_x != 0.0f || sp_y != 0.0f)
			{
				static Vector3d axis ;

				axis(sp_y, sp_x, 0.0f) ;
				axis.Unit() ;
				glRotatef(sqrt(sp_x * sp_x + sp_y * sp_y), axis.X(), axis.Y(), axis.Z()) ;
			}

			glMultMatrixf(aRotate) ;
			glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
		}
		glPopMatrix() ;

		glTranslatef(0.0f, 0.0f, n->clock_z) ;	// ������ư
		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -n->roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		DrawClock(*n) ;
	}
	glPopMatrix() ;
}

// ��ǥ����衢���󥻥åȡʥ���ƥ������ꥢ���󥰤Σ�������ܤ������
void DrawModelMatrixAnti(ClockState& st)
{
	glPushMatrix() ;
	{
		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, st.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -st.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		DrawClock(st) ;
	}
	glPopMatrix() ;
}


// ��ǥ����衢���󥻥åȡʥ���ƥ������ꥢ���󥰤Σ�������ܤ������
// �����Х���ץ����
void DrawModelMatrixAntiWithOverSampling(ClockState& st, int count)
{
	glPushMatrix() ;
	{
		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, st.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -st.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		DrawClock(st, count) ;
	}
	glPopMatrix() ;
}

// ��ǥ����衢���󥻥åȡʥǥץ����֥ե�����ɥ֥顼�Σ�������ܤ������
// �������졼�����Хåե��ե饰����
void DrawModelMatrixDepthOfField(ClockState& st, int count, int flagNoAccum = FALSE)
{
	// ����ܡ�count == 0�ˤλ��Τߡ���ž���������
	if (!count)
	{
		glPushMatrix() ;
		{
			glLoadIdentity() ;

			if (spin_x != 0.0f || spin_y != 0.0f)
			{
				static Vector3d axis ;

				axis(spin_y, spin_x, 0.0f) ;
				axis.Unit() ;
				glRotatef(sqrt(spin_x * spin_x + spin_y * spin_y), axis.X(), axis.Y(), axis.Z()) ;
			}

			glMultMatrixf(aRotate) ;
			glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
		}
		glPopMatrix() ;
	}

	// �ǥץ����֥ե�����ɥ֥顼�Ǥϡ�
	// ñ�̹���˥ӥ塼����Υ��åȤ�����ľ��
	glPushMatrix() ;
	{
		// �ǥץ����֥ե�������ѥ��å�����򸵤�
		// �ӥ塼���󥻥å�
		glLoadIdentity() ;
		gluLookAt(jitter_dof[count].x * dof_range,
				  jitter_dof[count].y * dof_range,
				  0.0f,
				  0.0f,0.0f,focus_distance, 0.0f,1.0f,0.0f) ;

		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, st.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -st.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		if (flagNoAccum)
			DrawClock(st, count) ;
		else
			DrawClock(st) ;
	}
	glPopMatrix() ;
}


// �饤�Ȥκ��٤��������ʤ��� lcol[][] �˥��å�
String SetLightColorTableFromSaturation()
{
	// �饤�Ȥκ��٤��������ʤ��� lcol[][] �˥��å�
	for (int l = 0 ; l < 8 ; l ++)
	{
		Vector3d work = ChangeSaturation(Vector3d(master_lcol[l]), light_vividness * LIGHT_MASTER_VIVIDNESS) ;
		lcol[l][0] = work.v[0] ;
		lcol[l][1] = work.v[1] ;
		lcol[l][2] = work.v[2] ;
		lcol[l][3] = 1.0f ;
	}

	// ������ɥ������ȥ륻�å�
	String title = "Light saturation: " ;
	if (vertexShadingFlag)
	{
		title.AddFormat("%3.1f", light_vividness) ;
		SetTemporaryWindowTitle(title) ;
	}

	return title ;
}

void SetLightsColor()
{
	// �����ܰʹߤΥ饤�ȤϴĶ���������
	switch (light)
	{
	case 8:
		SetLightColor(GL_LIGHT7, lcol[7], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT7 ���å�
	case 7:
		SetLightColor(GL_LIGHT6, lcol[6], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT6 ���å�
	case 6:
		SetLightColor(GL_LIGHT5, lcol[5], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT5 ���å�

	case 5:
		SetLightColor(GL_LIGHT4, lcol[4], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT4 ���å�
	case 4:
		SetLightColor(GL_LIGHT3, lcol[3], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT3 ���å�
	case 3:
		SetLightColor(GL_LIGHT2, lcol[2], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT2 ���å�
	case 2:
		SetLightColor(GL_LIGHT1, lcol[1], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT1 ���å�
	case 1:
		SetLightColor(GL_LIGHT0, lcol[0], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT0 ���å�
	}
}


// ʿ�Ը���
void SetDirectinalLights()
{
	// �����ܰʹߤΥ饤�ȤϴĶ���������
	switch (light)
	{
	case 8:
		SetDirectionalLight(GL_LIGHT7, ldir[7], lcol[7], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT7 ���å�
	case 7:
		SetDirectionalLight(GL_LIGHT6, ldir[6], lcol[6], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT6 ���å�
	case 6:
		SetDirectionalLight(GL_LIGHT5, ldir[5], lcol[5], 0.0f, specularFlag, bright, currentLC) ;	// LIGHT5 ���å�

	case 5:
		SetDirectionalLight(GL_LIGHT4, ldir[4], lcol[4], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT4 ���å�
	case 4:
		SetDirectionalLight(GL_LIGHT3, ldir[3], lcol[3], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT3 ���å�
	case 3:
		SetDirectionalLight(GL_LIGHT2, ldir[2], lcol[2], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT2 ���å�
	case 2:
		SetDirectionalLight(GL_LIGHT1, ldir[1], lcol[1], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT1 ���å�
	case 1:
		SetDirectionalLight(GL_LIGHT0, ldir[0], lcol[0], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright, currentLC) ;	// LIGHT0 ���å�
	}
}

// ���ʥ��ݥåȡ˸���
void SetPositionalLights()
{
	GLfloat spot_cutoff = 180.0f ;
	GLfloat spot_exp = 0.0f ;
	GLfloat *dir[8] = { NULL, NULL, NULL, NULL, NULL } ;

	if (lightType == LIGHT_TYPE_SPOT)
	{
		spot_cutoff = SPOT_LIGHT_CUTOFF ;
		spot_exp    = SPOT_LIGHT_EXPONENT ;
		for (int l = 0 ; l < 8 ; l ++)
			dir[l] = sdir[l] ;
	}

	glMatrixMode(GL_MODELVIEW) ;
	glPushMatrix() ;
	{
		glTranslatef(0.0f, 0.0f, B_DIST) ;

		// �����ܰʹߤΥ饤�ȤϴĶ���������
		switch (light)
		{
		case 8:
			SetPositionalLight(GL_LIGHT7, lpos[7], lcol[7], 0.0f, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[7],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT7 ���å�
		case 7:
			SetPositionalLight(GL_LIGHT6, lpos[6], lcol[6], 0.0f, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[6],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT6 ���å�
		case 6:
			SetPositionalLight(GL_LIGHT5, lpos[5], lcol[5], 0.0f, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[5],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT5 ���å�

		case 5:
			SetPositionalLight(GL_LIGHT4, lpos[4], lcol[4], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[4],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT4 ���å�
		case 4:
			SetPositionalLight(GL_LIGHT3, lpos[3], lcol[3], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[3],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT3 ���å�
		case 3:
			SetPositionalLight(GL_LIGHT2, lpos[2], lcol[2], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[2],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT2 ���å�
		case 2:
			SetPositionalLight(GL_LIGHT1, lpos[1], lcol[1], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[1],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT1 ���å�
		case 1:
			SetPositionalLight(GL_LIGHT0, lpos[0], lcol[0], ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN, dir[0],
							   spot_cutoff, spot_exp, currentLC) ;	// LIGHT0 ���å�
		}
	}
	glPopMatrix() ;
}


// ������ǥ���󡢸������ˤ�ä�
// ������ǥ��Ƹ����� Enable/Disable �򹹿�
void UpdateLightModel(int logFlag)
{
	if (!vertexShadingFlag) return ;

	// Mesa
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, TRUE) ;

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local) ;

	if (lightTwoSide)
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) ;
	else
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE) ;

	glDisable(GL_LIGHT0) ;
	glDisable(GL_LIGHT1) ;
	glDisable(GL_LIGHT2) ;
	glDisable(GL_LIGHT3) ;
	glDisable(GL_LIGHT4) ;

	glDisable(GL_LIGHT5) ;
	glDisable(GL_LIGHT6) ;
	glDisable(GL_LIGHT7) ;

	if (light >= 8) glEnable(GL_LIGHT7) ;
	if (light >= 7) glEnable(GL_LIGHT6) ;
	if (light >= 6) glEnable(GL_LIGHT5) ;

	if (light >= 5) glEnable(GL_LIGHT4) ;
	if (light >= 4) glEnable(GL_LIGHT3) ;
	if (light >= 3) glEnable(GL_LIGHT2) ;
	if (light >= 2) glEnable(GL_LIGHT1) ;
	if (light >= 1) glEnable(GL_LIGHT0) ;

	String log ;

	if (lightType == LIGHT_TYPE_DIRECTIONAL)
	{
		SetDirectinalLights() ;
		log = IntToString(light) + " directional light(s)" ;
	}
	else
	{
		SetPositionalLights() ;

		if (lightType == LIGHT_TYPE_POINT)
//			logprintf("%d point light(s)", light) ;
			log = IntToString(light) + " point light(s)" ;
		else
//			logprintf("%d spot light(s)", light) ;
			log = IntToString(light) + " spot light(s)" ;
	}

	if (local)
//		logprintf(" with local viewer.\n") ;
		log += " with local viewer" ;
	else
		log += "" ;

	if (logFlag)
	{
		logprintf(log + ".\n") ;
		SetTemporaryWindowTitle(log) ;
	}
}


// �������å�
void SetStaticLightModel()
{
	// Separate Specular Color
	if ((mode_solid >= 10 || mode_real == 13) && has_separate_specular_color) // || (mode_env == 2 && has_separate_specular_color))
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR) ;
}


// �������å�
void SetLightModel(int logFlag = TRUE)
{
	if (light == 0)
		ambLight = 0.0f ;
	else
		ambLight = 0.05f / light + 0.1f ;	// 0.15 �� 0.11
//		ambLight = 0.06f / light + 0.02f ;

//	specularFlag = (mode_normal || mode_solid >= 2 || mode_env >= 2) ;

//	bright = (11 - light) / 6.0f * masterBright * COLLECT_MASTER_BRIGHT ;
//	bright = (5.0f / light + 6.0f) / 6.0f * masterBright * COLLECT_MASTER_BRIGHT ;
	bright = Pow(2.0f, 2.0f - light * 0.125f) / Pow(2.0f, 2.0f - 5.0f * 0.125f)
		   * masterBright * COLLECT_MASTER_BRIGHT ;

	if (mode_normal) bright *= COLLECT_NORMAL_BRIGHT ;

	GLfloat	wamb[] = { ambLight * currentLC[0], ambLight * currentLC[1], ambLight * currentLC[2], 1.0f } ;

	if (mode_env == 1 || mode_solid == 1 || mode_crystal || mode_real == 1) return ;

	if (texture != 1 && (env != 2) && light)
		glEnable(GL_LIGHTING) ;

	// ���ɴĶ��� 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, wamb) ;

	// Separate Specular Color
//	if ((mode_solid >= 10 || mode_real == 13) && has_separate_specular_color) // || (mode_env == 2 && has_separate_specular_color))
//		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR) ;


	UpdateLightModel(logFlag) ;
}


/*

// FPS, kTPS, kTPF �ե����ޥå�
//
// �ʲ��򥳡��뤹��褦���ѹ�����ݤˤϡ�
// FORMAT_FPS, FORMAT_KTPS, FORMAT_KTPF �ζ�������Ƥ�����
String Format_FPS(float fps)
{
	String str ;

	if (fps != 0.0f)
	{
		char buf[32] ;
		sprintf(buf, FORMAT_FPS, fps) ;

		if (fps >= 1000.0f)
		{
			buf[0] = buf[1] ;
			buf[1] = ',' ;
		}

		str = buf ;
	}
	else
	{
		str = "_,___.__ FPS" ;
	}

	return str ;
}

String Format_kTPS(float kTPS = 0.0f)
{
	String str ;

	if (fps != 0.0f)
	{
		char buf[32] ;
		sprintf(buf, " " + FORMAT_KTPS, kTPS) ;

		if (kTPS >= 1000.0f)
		{
			buf[0] = buf[1] ;
			buf[1] = buf[2] ;
			buf[2] = ',' ;
		}

		str = buf ;
	}
	else
	{
		str = "__,___.___ kTPS" ;
	}

	return str ;
}

String Format_kTPF(float kTPF = 0.0f)
{
	String str ;

	if (fps != 0.0f)
	{
		char buf[32] ;
		sprintf(buf, " " + FORMAT_KTPF, kTPF) ;

		if (kTPF >= 1000.0f)
		{
			buf[0] = buf[1] ;
			buf[1] = ',' ;
		}

		str = buf ;
	}
	else
	{
		str = "_,___.___ kTPS" ;
	}

	return str ;
}

String Format_FPS_kTPS(float fps = 0.0f, float kTPS = 0.0f)
{
	String str ;

	str = Format_FPS(fps) ;
	str += " " + Format_kTPS(kTPS) ;

	return str ;
}

String Format_FPS_kTPS_kTPF(float fps = 0.0f, float kTPS = 0.0f, float kTPF = 0.0f) ;
{
	String str ;

	str = Format_FPS(fps) ;
	str += " " + Format_kTPS(kTPS) ;
	str += "   at " + Format_kTPS(kTPF) ;

	return str ;
}

*/


int NLightsOfPerVertexLightingMark(int testCount)
{
	int nLights ;
	int l = testCount % 4 ;	// 0,1,2,3, 0,1,2,3, ...
	if (l == 0)
		nLights = 1 ;
	else
		nLights = Pow(2, l) ;	// 1,2,4,8, 1,2,4,8, ...

	//	nLights = ((testCount % 4) + 1) * 2 ;	// 2,4,6,8, 2,4,6,8, ...

	return nLights ;
}


void SetFogParameter()
{
	if (fogFlag == FOG_LINEAR)
	{
		fog_mode  = GL_LINEAR ;
		logprintf("Linear equation fogging.\n") ;
		SetTemporaryWindowTitle("Linear equation fogging") ;
	}
	else if (fogFlag == FOG_EXP)
	{
		fog_mode = GL_EXP ;
		logprintf("1 exponential equation fogging.\n") ;
		SetTemporaryWindowTitle("1 exponential equation fogging") ;
	}
	else if (fogFlag == FOG_EXP2)
	{
		fog_mode = GL_EXP2 ;
		logprintf("2 exponential equation fogging.\n") ;
		SetTemporaryWindowTitle("2 exponential equation fogging") ;
	}

	if (maxTriTest)
	{
		fog_density	= FOG_DENSITY / FOG_MAX_RATIO ;
		fog_start   = FOG_START ;
		fog_end     = FOG_END * FOG_MAX_RATIO ;
	}
	else
	{
		fog_density	= FOG_DENSITY ;
		fog_start   = FOG_START ;
		fog_end     = FOG_END ;
	}
}


// �ե������å�
void SetFog()
{
	if (fogFlag != FOG_NONE)
	{
		SetFogParameter() ;

		glFogfv(GL_FOG_COLOR,	currentBG) ;
		glFogf(GL_FOG_MODE,		fog_mode) ;
		glFogf(GL_FOG_START,	fog_start) ;
		glFogf(GL_FOG_END,		fog_end) ;
		glFogf(GL_FOG_DENSITY,	fog_density) ;

		glEnable(GL_FOG) ;
	}
	else
	{
			glDisable(GL_FOG) ;
		logprintf("Non fogging.\n") ;
		SetTemporaryWindowTitle("Non fogging") ;
	}
}


// glclockmark �⡼�ɤν������
// Benchmark�������ȥ륻�åȡ�
void InitGlClockMark(float glclock_mark)
{
	if (glclock_mark == 0.0f) return ;

	benchmark = TRUE ;

	if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEST)
	{
		rotateLight = TRUE ;
		lightType = LIGHT_TYPE_DIRECTIONAL ;
		light = NLightsOfPerVertexLightingMark(testCount) ;
		local = FALSE ;
		light_vividness = LIGHT_MARK_VIVIDNESS ;
		g_smooth = TRUE ;	// -gsmooth ON
//		title = lightTestTitle[0] ;
		tmpTitle = lightTestTitle[0] ;
	}
	else if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST ||
			 glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST)
	{
		if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST)
			solid = 2 ;
		else
			solid = 3 ;

		rotateLight = TRUE ;
		lightType = 0 ;
		light = 5 ;
		local = FALSE ;
		light_vividness = LIGHT_MARK_VIVIDNESS ;
//		title = lightTexTestTitle[0] ;
		tmpTitle = lightTexTestTitle[0] ;
	}
	else if (glclock_mark == (float)NO_GLCLOCK_MARK_FILTER_TEST)
	{
		filterTest = TRUE ;
		solid = 1 ;
//		title = filterTestTitle[0] ;
		tmpTitle = filterTestTitle[0] ;

		// �ǽ�ϥݥ���ȥ���ץ��
		SetTextureFilter("N") ;
//		gl_mag_filter = GL_NEAREST ;
//		gl_min_filter = GL_NEAREST ;
	}
	else if (glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEST ||
			 glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEX_TEST)
	{
		maxTriTest = TRUE ;	// Max Test Mode

		if (glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEST)
			light = 0 ;		// �饤�ȥ���
		else
			solid = 1 ;

//		title = fogTestTitle[0] ;
		tmpTitle = fogTestTitle[0] ;

		// �ǽ�ϥե����ʤ�
		fogFlag = FOG_NONE ;
	}
}


// glclockmark �ƥ��ȣ�
// Per-Vertex Lighting Mark
String PerVertexLightingMark()
{
	testCount ++ ;

	// �����ȥ�����ϡʣ������Ρ���ʸ��������
	char buf[32] ;
	sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_LIGHT_TEST + testCount * 0.01)) ;
	String markLog = buf + lightTestTitle[testCount - 1] + "\n" ;

	light = NLightsOfPerVertexLightingMark(testCount) ;

	if (testCount >= 0 && testCount < 4)
	{
		// testCount �������ʥƥ��ȣ������ˤ�����
		local = GL_FALSE ;
		lightType = LIGHT_TYPE_DIRECTIONAL ;
	}
	else if (testCount >= 4 && testCount < 8)
	{
		// testCount �������ʥƥ��ȣ������ˤ�����
		local = GL_TRUE ;
		lightType = LIGHT_TYPE_DIRECTIONAL ;
	}
	else if (testCount >= 8 && testCount < 12)
	{
		// testCount ���������ʥƥ��ȣ��������ˤ�����
		local = GL_FALSE ;
		lightType = LIGHT_TYPE_POINT ;
	}
	else if (testCount >= 12 && testCount < 16)
	{
		// testCount �����������ʥƥ��ȣ����������ˤ�����
		local = GL_TRUE ;
		lightType = LIGHT_TYPE_POINT ;
	}
	else if (testCount >= 16 && testCount < 20)
	{
		// testCount �����������ʥƥ��ȣ����������ˤ�����
		local = GL_FALSE ;
		lightType = LIGHT_TYPE_SPOT ;
	}
	else if (testCount >= 20 && testCount < 24)
	{
		// testCount �����������ʥƥ��ȣ����������ˤ�����
		local = GL_TRUE ;
		lightType = LIGHT_TYPE_SPOT ;
	}

	if (testCount >= N_GLCLOCK_MARK_LIGHT_TEST)
	{
		testCount = 0 ;
		return markLog ;
	}

	// �����ȥ�ϼ��Υƥ��Ȥ�
	title = lightTestTitle[testCount] ;
	glutSetWindowTitle(title) ;
	// ��������
	SetLightModel() ;

	return markLog ;
}


// glclockmark �ƥ��ȣ�
// Per-Vertex Lighting Texture Mark
String PerVertexLightingTextureMark()
{
	testCount ++ ;

	// �����ȥ�����ϡʣ������Ρ���ʸ��������
	char buf[32] ;
	if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST)
		sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_LIGHT_TEX_TEST + testCount * 0.01)) ;
	else
		sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST + testCount * 0.01)) ;

	String markLog = buf + lightTexTestTitle[testCount - 1] + "\n" ;

	if (mode_solid >= 3)
	{
		if (mode_solid >= 10)
			markLog += "1-pass (" + separate_specular_color_name + ") - " ;
		else
			markLog += "2-pass (Diffuse and Specular) - " ;
	}

	// �饤�ȿ��ϣ�����
	light = 5 ;
	if ((testCount % 2) == 0)
		local = GL_FALSE ;
	else
		local = GL_TRUE ;

	if (testCount >= 0 && testCount < 2)
		lightType = LIGHT_TYPE_DIRECTIONAL ;

	else if (testCount >= 2 && testCount < 4)
		lightType = LIGHT_TYPE_POINT ;

	else if (testCount >= 4 && testCount < 6)
		lightType = LIGHT_TYPE_SPOT ;

	if (testCount >= N_GLCLOCK_MARK_LIGHT_TEX_TEST)
	{
		testCount = 0 ;
		return markLog ;
	}

	// �����ȥ�ϼ��Υƥ��Ȥ�
	title = lightTexTestTitle[testCount] ;
	glutSetWindowTitle(title) ;
	// ��������
	SetLightModel() ;

	return markLog ;
}


// glclockmark �ƥ���
String FoggingMark()
{
	testCount ++ ;

	// �����ȥ�����ϡʣ������Ρ���ʸ��������
	char buf[32] ;
	if (glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEST)
		sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_FOG_TEST + testCount * 0.01)) ;
	else
		sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_FOG_TEX_TEST + testCount * 0.01)) ;

	String markLog = buf + fogTestTitle[testCount - 1] + "\n" ;

	if (testCount == 1)	// �����ܤ� Linear
		fogFlag = FOG_LINEAR ;

	if (testCount == 2)	// �����ܤ� EXP
		fogFlag = FOG_EXP ;

	else if (testCount == 3)	// �����ܤ� EXP2
		fogFlag = FOG_EXP2 ;

	if (testCount >= N_GLCLOCK_MARK_FOG_TEST)
	{
		testCount = 0 ;
		return markLog ;
	}

	// �����ȥ�ϼ��Υƥ��Ȥ�
	title = fogTestTitle[testCount] ;
	glutSetWindowTitle(title) ;
	// �ե�������
	SetFog() ;

	return markLog ;
}


// glclockmark �ƥ��ȣ�
// Texture Filtering Mark
String TextureFilteringMark()
{
	testCount ++ ;

	// �����ȥ�����ϡʣ������Ρ���ʸ��������
	char buf[32] ;
	sprintf(buf, "%3.2f ", (NO_GLCLOCK_MARK_FILTER_TEST + testCount * 0.01)) ;

	String markLog = buf + filterTestTitle[testCount - 1] + "\n" ;

	if (testCount == 1)
	{
		// �ߥåץޥå�
		SetTextureFilter("NN") ;
	}
	else if (testCount == 2)
	{
		// �Х���˥�
		SetTextureFilter("L") ;
	}
	else if (testCount == 3)
	{
		// �Х���˥��ܥߥåץޥå�
		SetTextureFilter("LN") ;
	}
	else if (testCount == 4)
	{
		// �ȥ��˥�
		SetTextureFilter("LL") ;
	}

	if (testCount >= N_GLCLOCK_MARK_FILTER_TEST)
	{
		testCount = 0 ;
		return markLog ;
	}

	// �����ȥ�ϼ��Υƥ��Ȥ�
	title = filterTestTitle[testCount] ;
	glutSetWindowTitle(title) ;
	// �ե��륿����
	UpdateOpenGLTextureFilter() ;
//	UpdateOpenGLAnisotropy(maxAnisotropy) ;

	return markLog ;
}


String ControlGlClockMark(float glclock_mark)
{
	String markLog ;
	markLog = pre_fps_message ;

	if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEST)
		markLog = PerVertexLightingMark() ;

	else if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST ||
			 glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST)
		markLog = PerVertexLightingTextureMark() ;

	else if (glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEST ||
			 glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEX_TEST)
		markLog = FoggingMark() ;

	else if (glclock_mark == (float)NO_GLCLOCK_MARK_FILTER_TEST)
		markLog = TextureFilteringMark() ;

	else
	{
		// �������ʤ��ƥ����ֹ�
		if (markLog != "")
		{
			// glclock mark �ϤΥƥ��Ȥʤ�
			markLog += "\n" ;

			if (mode_real)
			{
				// -r ���ץ������ϥޥ���ƥ�������Ϥ�������ɲ�
				if (detail == 1)
					markLog += "max 3-pass 3-texturing - " ;
				else if (detail == 2)
					markLog += "max 2-pass 3-texturing (" + multitexture_name + ") - " ;
				else if (detail == 3)
					markLog += "max 1-pass 3-texturing (" + multitexture_name + ") - " ;

				// !detail
				else if (mode_real >= 10)
					markLog += "max 1-pass 2-texturing (" + multitexture_name + ") - " ;
				else
					markLog += "max 2-pass 2-texturing - " ;
			}
		}

		// testCount �˥���������Ƚ�λ
		testCount = 0 ;
//		return markLog ;
	}

	if (markLog != "")
	{
		markLog.AddFormat("%8.3f kTris/Frame - ", avg_ktris) ;

		if (glut_buff == GLUT_SINGLE)
			markLog += "Single Buffering\n" ;
		else
			markLog += "Double Buffering\n" ;

		// ��Ƭ�˼��̻Ҥ�Ĥ���
		markLog = "#" + markLog ;
	}

	firstFPS = TRUE ;
	fps_count = -1 ;
	max_ktps = 0.0 ;
	max_fps  = 0.0 ;
	wait_calc_fps = 0 ; //FPS_CALC_WAIT ;		// �����ܰʹߤΥѥ�᥿�ڤ��ؤ����������ȤϤʤ�

	return markLog ;
}


// ���Ѳ�ž��������
void InitRotate()
{
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;

	if (saver)
		glRotatef(SAVER_ROTATE_Y, 0.0f,1.0f,0.0f) ;
	else if (benchmark)
		glRotatef(BENCH_ROTATE_Y, 0.0f,1.0f,0.0f) ;
	else
		glRotatef(ROTATE_Y, 0.0f,1.0f,0.0f) ;

	glRotatef(ROTATE_X, 1.0f,0.0f,0.0f) ;

#ifdef GIFF_DEMO
	glRotatef(-6.0f, 0.0f,1.0f,0.0f) ;
#endif

	glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
}


void InitRotateMatrix()
{
/*
	static GLfloat nowMatrix[16] ;
	glGetFloatv(GL_MODELVIEW_MATRIX, nowMatrix) ;

	InitRotate() ;
	glTranslatef(nowMatrix[12], nowMatrix[13], nowMatrix[14]) ;	// ������ư
//	aRotate[3]= nowMatrix[3] ;
//	aRotate[7]= nowMatrix[7] ;
//	aRotate[11]= nowMatrix[11] ;
*/

	glPushMatrix() ;
	{
		InitRotate() ;
		glTranslatef(0.0f, 0.0f, B_DIST) ;
	}
	glPopMatrix() ;
}


void AdjustSpinAndFrameCount(float& spin_x, int& dst_fps_count, double fps)
{
	if (!benchmark)
	{
		spin_x = SPIN_X ;
		return ;
	}

	if (fps == 0.0)
	{
		dst_fps_count = (int)(-360.0f / SPIN_X_BENCH) ;
		spin_x = -360.0f / (float)dst_fps_count ;

		// ����Υƥ��ȤǤϣ���ž�ǰ��׻�
		if (filterTest || maxTriTest || focusTest)
			dst_fps_count *= 2 ;

		return ;	// �������Ȼ��Ϥ��٤�Ʊ��®�٤Τޤ�
	}

	dst_fps_count = (int)((float)fps * 360.0 / master_spin_ratio) ;

	if (dst_fps_count < 8)
		dst_fps_count = (int)(-360.0f / SPIN_X_BENCH) ;	// ����Ǥ⣸��ʣ����٤��ġ�
	else if (dst_fps_count > 2880)
		dst_fps_count = 2880 ;		// ¿���Ȥ⣲��������� 0.125 �٤��ġ�

	// filterTest ���ϲ�ž®��Ⱦʬ
//	if (filterTest)
//		dst_fps_count *= 2 ;

	spin_x = -360.0f / (float)dst_fps_count ;

	// ����Υƥ��ȤǤϣ���ž�ǰ��׻�
	if (filterTest || maxTriTest || focusTest)
		dst_fps_count *= 2 ;
}


/*
void AdjustSpinAndFrameCount(float& spin_x, int& dst_fps_count)
{
	if (filterTest)
	{
//		spin_x *= 0.5f ;
		dst_fps_count = (int)(-360.0f / spin_x) * 2 ;
	}
	else if (maxTriTest)
	{
//		spin_x *= 0.5f ;
		dst_fps_count = (int)(-360.0f / spin_x) * 2 ;
	}
	else if (focusTest)
	{
//		spin_x *= 0.5f ;
		dst_fps_count = (int)(-360.0f / spin_x) * 2 ;
	}
	else
		dst_fps_count = (int)(-360.0f / spin_x) ;
}
*/


// FPS�η׻�
void FramesPerSecond()
{
	static int  n_tick = 0, o_tick = 0 ;
	static char c_fps [32] = { "____.__ FPS" } ;
	static char c_tfps[64] = { "____.__ FPS _____.___ kTPS" } ;

	master_fps_count ++ ;
	if (changeTitleWait)
		changeTitleWait -- ;

//	Sleep((int)((float)rand() / RAND_MAX) * 100) ;


	o_tick = n_tick ;
//	n_tick = glutGet((GLenum)GLUT_ELAPSED_TIME) ;
	n_tick = GetMiliSeconds() ;


//	logprintf("%d\n", n_tick - o_tick) ;


	if (n_tick < startElapsedTime)
		firstFPS = TRUE ;	// ǰ�Τ���

	if (firstFPS)
	{
		firstFPS = FALSE ;
		startElapsedTime = n_tick ;
	}

	if (fps_count < 0)
	{
		// �ǽ�� FramesPerSecond() �������뤵��Ƥ��������ַвᤷ�Ƥ���׻�����
//		if ((n_tick - startElapsedTime >= wait_calc_fps && loop_count >= (-360.0 * 2.0 / SPIN_X_BENCH) - 2))
		if ((n_tick - startElapsedTime >= wait_calc_fps && loop_count >= (-360.0 * 2.0 / SPIN_X_BENCH) - 2) ||
			(n_tick - startElapsedTime >= wait_calc_fps * 2 && loop_count >= (-360.0 / SPIN_X_BENCH) - 2))
		{
			if (testCount)
				fps_count = 1 ;
			else
			{
				fps_count = 0 ;

				// �ǽ�Υƥ��Ȥξ�硢�������Ȥ�Ʊ���˥����ȥ��ѹ�
				if (tmpTitle != "")
				{
					title = tmpTitle ;
					glutSetWindowTitle(title) ;
				}
			}
		}
	}
	else
	{
		fps_count ++ ;
	}

	if (n_tick && (n_tick > o_tick))
		fps = f_cnt / ((double)(n_tick - o_tick) / 1000) ;
	else
		fps = 0.0 ;

#ifdef WIN32
	if (g_pFps) sprintf(g_pFps, "%3.2f", fps) ;

	// Windows �ξ�硢
	// �����꡼�󥻡��Х⡼�ɤξ��ϡ�
	// Window �򥢥��ƥ��֤ˤ��ʤ���
//	if (voodoo && saver && hWin)
	if (saver && hWin)
		SetActiveWindow(hWin) ;
#endif

//	kTPS = r_polygons * fps * 0.001 ;
	avg_ktris = (double)f_polygons / f_cnt * 0.001 ;	// ʿ�Ѥ�����ݥꥴ����������󤫤������ݥꥴ�����������󤫤�Υե졼���
	kTPS = avg_ktris * fps ;							// ��ԣУӡ�ʿ�ѤΥݥꥴ�������ߣƣУ�
	if (fps_count >= 1)
	{
		sprintf(c_fps,  f_FPS, fps) ;
		sprintf(c_tfps, f_FPS_kTPS, fps, kTPS) ;

		if (f_prn)
			logprintf(f_FPS_kTPS_kTPF+ "\n", fps, kTPS, avg_ktris) ;
	}

	s_fps = c_fps ;
	t_fps = c_tfps ;

	// ������ɥ������ȥ������
	if (!saver && fps > 0.0f && fps_count >= 1 && !benchmark)
		SetWindowTitle(t_fps + ".  " + title) ;

/*
	// CPU �ѥե����ޥ󥹤����ꤵ��Ƥ������
	// FPS �˱����� us_sleep �ͤ�Ʒ׻�
	if (sleepRatio > 0.0f && fps > 0.0f)
	{
		// ���꡼�פ��ʤ��ä����ˣ��������ˤ����ä��ߥ���
		double msecond = 1000.0f / fps - us_sleep ;
		if (msecond > 0.0f)
			us_sleep = (unsigned int)((1.0f / sleepRatio - 1.0f) * msecond) ;
		else
			us_sleep = 0 ;
	}
*/

	// �٥���ޡ����¹Ի�����
	if (benchmark && fps_count >= 0)
	{
		InitRotateMatrix() ;

		// ¬���SPIN_X_BENCH ��®�٤ǰ��ž�ˣ��ðʾ夫���äƤ������
		AdjustSpinAndFrameCount(spin_x, dst_fps_count, fps) ;
/*
			// timerResolution �ˤ�ä��ѹ���
			spin_x = - master_spin_ratio / fps ;

			if (-spin_x <= 8.0f)
				spin_x = (float)((int)spin_x) ;		// ����ʲ��ʤ飱���ܿ���

			else if (-spin_x <= 18.0f)
				spin_x = (float)((int)(spin_x / 3) * 3.0f) ;	// ������ʲ��ʤ飳���ܿ���

			else if (-spin_x <= 36.0f)
				spin_x = (float)((int)(spin_x / 6) * 6.0f) ;	// ������ʤ飶���ܿ���

			else
				spin_x = -45.0f ;	// ���������礭�����ϣ�����

//			Sleep(500) ;
			if (spin_x > -1.0f)
				spin_x = -1.0f ;	// ����������Ǥ⣱�٤��Ĥϲ�ž
//			else if (spin_x < SPIN_X_BENCH)
//				spin_x = SPIN_X_BENCH ;		// �ǹ�Ǥ� SPIN_X_BENCH ��Ʊ��
		}
		else
		{
			spin_x = SPIN_X_BENCH ;
		}

		if (filterTest)
			spin_x *= 0.5f ;
//			spin_x *= 0.25f ;
//		else if (maxTriTest)
//			spin_x *= 0.5f ;
//		else if (focusTest)
//			spin_x *= 0.5f ;

		AdjustSpinAndFrameCount(spin_x, dst_fps_count) ;
*/
	}

	if (benchmark && fps_count >= 1)
	{
/*
		if (fps_count >= 1)
		{
			if (fps)
				spin_x = -360.0f / fps ;
			else
				spin_x = -30.0f ;

			if (spin_x < -30.0f) spin_x = -30.0f ;

			// ������ɥ������ȥ������
			if (!saver)
				glutSetWindowTitle(s_fps) ;
		}
*/

		static FPSInfo fpsInfo[10] ;
		fpsInfo[fps_count - 1].frameCount   = f_cnt ;
		fpsInfo[fps_count - 1].mSecond      = n_tick - o_tick ;
		fpsInfo[fps_count - 1].polygonCount = f_polygons ;

		fpsInfo[fps_count - 1].fps = fps ;
		fpsInfo[fps_count - 1].kTPS = kTPS ;

		fpsInfo[fps_count - 1].weightFPS  = fps  * fpsInfo[fps_count - 1].mSecond ;
		fpsInfo[fps_count - 1].weightKTPS = kTPS * fpsInfo[fps_count - 1].mSecond ;

		// FPS �� ������ݥꥴ���������
		sum_fps += fps ;
		sum_polygons += f_polygons ;

		// ��������¸�ʣ��á����ʿ���ͤκ����͡�
		if (fps > max_fps)
		{
			max_fps  = fps ;
			max_ktps = kTPS ;
		}

		int finishFlag = FALSE ;
/*
		if (testCount == 0)
		{
			// �̾����������Ȥ����ʾ塢�ޤ��ϡ�������ȣ��Ǥ⡢��ư���Ƥ��飱���ðʾ�вᤷ�Ƥ�����
			// -FT ����������Ȥ����ʾ塢�ޤ��ϡ�������ȣ��Ǥ⡢��ư���Ƥ��飱���ðʾ�вᤷ�����
			if (
				(!(filterTest || maxTriTest) && (fps_count >= 5 || (fps_count >= 2 && n_tick - startElapsedTime >= 15000))) ||
				( (filterTest || maxTriTest) && (fps_count >= 6 || (fps_count >= 4 && n_tick - startElapsedTime >= 15000)))
			   )
			   finishFlag = TRUE ;
		}
		else
		{
			// testCount >= 1 �ʹߤǤ�
			// �̾����������Ȥ����ʾ�ޤ������Υƥ��Ȥη�¬���飶�ðʾ�
			// -FT ����������Ȥ����ʾ�ޤ��ϣ��ʾ�����Υƥ��Ȥ��飱���ðʾ�
			if (
				(!(filterTest || maxTriTest) && (fps_count >= 4 || (fps_count >= 1 && n_tick - startElapsedTime >=  6000))) ||
				( (filterTest || maxTriTest) && (fps_count >= 4 || (fps_count >= 2 && n_tick - startElapsedTime >= 10000)))
			   )
			   finishFlag = TRUE ;
		}
*/
		if (testCount == 0)
		{
			// �̾����������Ȥ����ʾ�ޤ���
			// ������ȣ��ʾ��¬�곫�Ϥ��Ƥ��飱���ðʾ�вᤷ�Ƥ�����
			// ������ȣ��ʾ��¬�곫�Ϥ��Ƥ��飲���ðʾ�вᤷ�Ƥ�����
			if (fps_count >= 5 ||
				(fps_count >= 2 && n_tick - startElapsedTime >= 10000) ||
				(fps_count >= 1 && n_tick - startElapsedTime >= 20000))
			   finishFlag = TRUE ;
		}
		else
		{
			// testCount >= 1 �ʹߤǤ�
			// �̾����������Ȥ����ʾ�ޤ������Υƥ��Ȥη�¬���飵�ðʾ�
			if (fps_count >= 4 ||
				(fps_count >= 1 && n_tick - startElapsedTime >= 5000))
			   finishFlag = TRUE ;
		}

		if (finishFlag)
		{
			static String fstr ;
			if (testCount == 0)
				fstr = "" ;

			static FILE *fp ;
			double total_fps ;		// ���á����ʿ���ͤκ�����
			double total_ktps ;		// ���á����ʿ�ѥݥꥴ�������κ�����

//			total_fps  = max_fps ;		// ���á����ʿ���ͤκ�����
//			total_ktps = max_ktps ;		// ���á����ʿ�ѥݥꥴ�������κ�����

			// ���֤���� fps_count ��ʬ�ݣ���ʿ���ͤ�׻�����
			// ʿ���ͤ����ʾ��㤤�ͤ��˴�
			// �Ĥ��ʿ���ͤ�����Ѥ˺���
			{
				int nFPS = fps_count ;
				int sum_mSecond = 0 ;
				double sumFPS = 0.0 ;
				double sumKTPS = 0.0 ;
				for (int i = 0 ; i < nFPS ; i ++)
				{
					sum_mSecond += fpsInfo[i].mSecond ;

					sumFPS += fpsInfo[i].weightFPS ;
					sumKTPS += fpsInfo[i].weightKTPS ;
				}

//				total_fps = sumFPS / fps_count ;
				total_fps = sumFPS / sum_mSecond ;

				// ʿ���ͤ���Υ��Ƥ����ͤ��˴�
				for (int i = 0 ; i < nFPS ; i ++)
				{
//					if (fpsInfo[i].weightFPS < total_fps * 0.93 || fpsInfo[i].weightFPS > total_fps * 1.15)
					if (fpsInfo[i].fps < total_fps * 0.93 || fpsInfo[i].fps > total_fps * 1.15)
					{
						nFPS -- ;
						sum_mSecond -= fpsInfo[i].mSecond ;
						sumFPS  -= fpsInfo[i].weightFPS ;
						sumKTPS -= fpsInfo[i].weightKTPS ;
					}
				}

				// ʿ���ͤ�Ʒ׻�
				if (nFPS > 0)
				{
					total_fps  = sumFPS  / sum_mSecond ;
					total_ktps = sumKTPS / sum_mSecond ;
				}
				else
				{
					total_fps  = max_fps ;		// ���á����ʿ���ͤκ�����
					total_ktps = max_ktps ;		// ���á����ʿ�ѥݥꥴ�������κ�����
				}
			}

			logprintf(f_FPS_kTPS_kTPF + "\n\n", total_fps, total_ktps, avg_ktris) ;

			String markLogString = ControlGlClockMark(glclock_mark) ;

			// -foutfps ���ץ����ǥե����뤬���ꤵ�줿���ϡ��ե������FPS���ɲå⡼�ɤǽ���
			if (fout_fps != String(""))
				fstr.AddFormat(markLogString + d_FPS_kTPS_kTPF + "\n\n", total_fps, total_ktps, avg_ktris) ;

			if (testCount == 0)
			{
				// -foutfps ���ץ����ǥե����뤬���ꤵ�줿���ϡ��ե������FPS���ɲå⡼�ɤǽ���
				if (fstr != String(""))
				{
					if (!(fp = fopen(fout_fps, "a")))
					{
#ifdef WIN32
						MessageBox(NULL, "Can't open output file \"" + fout_fps + "\"", "File Error", MB_OK | MB_ICONSTOP) ;
#endif
						errprintf(String("Can't open output file \"") + fout_fps + "\"\n") ;
						EndProc() ;
						glclock_exit(EXIT_FAILURE) ;
					}

					fprintf(fp, fstr) ;
					fclose(fp) ;
				}

				EndProc() ;
				glclock_exit(EXIT_SUCCESS) ;
			}
		}

		else
		{
			// ������ɥ������ȥ������
//			glutSetWindowTitle(s_fps) ;
			if (maxTriTest)
				glutSetWindowTitle(t_fps + ".  " + title) ;
			else
				glutSetWindowTitle(s_fps + ".  " + title) ;
		}
	}


	f_cnt = f_polygons = 0 ;
	f_cal_flag = FALSE ;

	timeData.fp_hour   = timeData.n_hour ;
	timeData.fp_minute = timeData.n_minute ;
	timeData.fp_second = timeData.n_second ;

	// FPS �׻���ɽ��ʸ����������������ɥ������ȥ��ѹ����Υ����Хإåɤ��θ
//	n_tick = glutGet((GLenum)GLUT_ELAPSED_TIME) ;
//	n_tick = GetMiliSeconds() ;
}


// ���ߤλ��֤��顢�£ǥ��顼�����
void SetCurrentBackGroundColor()
{
	// �롢ͼ������ο�����֤����
	float cereal = timeData.t_area->tm_hour + timeData.n_minute / 60.0f + timeData.n_second / 3600.0f, ratio, aratio ;

	// �����꡼�󥻡��Х⡼�ɤ⤷���ϡ����ꥹ����� bg_color == 0�ʥǥե���ȡˤξ�硢�طʿ��Ϲ��Τޤ��Ѳ��ʤ�
	int   notGradBGFlag = (saver || (crystal && !bg_color)) ;

	if (cereal >= 5.5f && cereal < 6.0f)
	{
		grad_flag = TRUE ;
		aratio = 1.0f - (ratio = (cereal - 5.5f) * 2.0f) ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[1].v[0] * ratio + bgColor[2].v[0] * aratio ;
			currentBG[1] = bgColor[1].v[1] * ratio + bgColor[2].v[1] * aratio ;
			currentBG[2] = bgColor[1].v[2] * ratio + bgColor[2].v[2] * aratio ;
		}

		currentLC[0] = lightCl[1].v[0] * ratio + lightCl[2].v[0] * aratio ;
		currentLC[1] = lightCl[1].v[1] * ratio + lightCl[2].v[1] * aratio ;
		currentLC[2] = lightCl[1].v[2] * ratio + lightCl[2].v[2] * aratio ;
	}
	else if (cereal >= 6.0f && cereal <= 6.5f)
	{
		grad_flag = TRUE ;
		aratio = 1.0f - (ratio = (cereal - 6.0f) * 2.0f) ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[0].v[0] * ratio + bgColor[1].v[0] * aratio ;
			currentBG[1] = bgColor[0].v[1] * ratio + bgColor[1].v[1] * aratio ;
			currentBG[2] = bgColor[0].v[2] * ratio + bgColor[1].v[2] * aratio ;
		}

		currentLC[0] = lightCl[0].v[0] * ratio + lightCl[1].v[0] * aratio ;
		currentLC[1] = lightCl[0].v[1] * ratio + lightCl[1].v[1] * aratio ;
		currentLC[2] = lightCl[0].v[2] * ratio + lightCl[1].v[2] * aratio ;
	}
	else if (cereal >= 17.5f && cereal < 18.0f)
	{
		grad_flag = TRUE ;
		aratio = 1.0f - (ratio = (cereal - 17.5f) * 2.0f) ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[1].v[0] * ratio + bgColor[0].v[0] * aratio ;
			currentBG[1] = bgColor[1].v[1] * ratio + bgColor[0].v[1] * aratio ;
			currentBG[2] = bgColor[1].v[2] * ratio + bgColor[0].v[2] * aratio ;
		}

		currentLC[0] = lightCl[1].v[0] * ratio + lightCl[0].v[0] * aratio ;
		currentLC[1] = lightCl[1].v[1] * ratio + lightCl[0].v[1] * aratio ;
		currentLC[2] = lightCl[1].v[2] * ratio + lightCl[0].v[2] * aratio ;
	}
	else if (cereal >= 18.0f && cereal <= 18.5f)
	{
		grad_flag = TRUE ;
		aratio = 1.0f - (ratio = (cereal - 18.0f) * 2.0f) ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[2].v[0] * ratio + bgColor[1].v[0] * aratio ;
			currentBG[1] = bgColor[2].v[1] * ratio + bgColor[1].v[1] * aratio ;
			currentBG[2] = bgColor[2].v[2] * ratio + bgColor[1].v[2] * aratio ;
		}

		currentLC[0] = lightCl[2].v[0] * ratio + lightCl[1].v[0] * aratio ;
		currentLC[1] = lightCl[2].v[1] * ratio + lightCl[1].v[1] * aratio ;
		currentLC[2] = lightCl[2].v[2] * ratio + lightCl[1].v[2] * aratio ;
	}
	else if (cereal > 18.5f || cereal < 5.5f)
	{
		grad_flag = FALSE ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[2].v[0] ;
			currentBG[1] = bgColor[2].v[1] ;
			currentBG[2] = bgColor[2].v[2] ;
		}

		currentLC[0] = lightCl[2].v[0] ;
		currentLC[1] = lightCl[2].v[1] ;
		currentLC[2] = lightCl[2].v[2] ;
	}
	else
	{
		grad_flag = FALSE ;

		if (!notGradBGFlag)
		{
			currentBG[0] = bgColor[0].v[0] ;
			currentBG[1] = bgColor[0].v[1] ;
			currentBG[2] = bgColor[0].v[2] ;
		}

		currentLC[0] = lightCl[0].v[0] ;
		currentLC[1] = lightCl[0].v[1] ;
		currentLC[2] = lightCl[0].v[2] ;
	}

	// �Ȥꤢ�������Τޤ�
	currentCL[0] = currentLC[0] ;
	currentCL[1] = currentLC[1] ;
	currentCL[2] = currentLC[2] ;

	// �����׽�����Mesa��
	if (mesa)
	{
		if (currentCL[0] > 1.0f) currentCL[0] = 1.0f ;
		if (currentCL[1] > 1.0f) currentCL[1] = 1.0f ;
		if (currentCL[2] > 1.0f) currentCL[2] = 1.0f ;
	}

	currentGC[0] = currentCL[0] ;
	currentGC[1] = currentCL[1] ;
	currentGC[2] = currentCL[2] ;

	// �ƥ�������֥����ѤΥ����ȥ��顼�ʥ��ڥ���顼�ѡ�
	if (reality >= 10 || mode_env == 2)
	{
		currentSP[0] = 1.0 + (currentCL[0] - 1.0) * specularTimeGradRatio ;
		currentSP[1] = 1.0 + (currentCL[1] - 1.0) * specularTimeGradRatio ;
		currentSP[2] = 1.0 + (currentCL[2] - 1.0) * specularTimeGradRatio ;
		currentSP[3] = 1.0f ;
	}

	// ���顼���å�
	glClearColor(currentBG[0], currentBG[1], currentBG[2], currentBG[3]) ;
	glFogfv(GL_FOG_COLOR, currentBG) ;

	if (!vertexShadingFlag) return ;

/*
	// LIGHT0 �ο����å�
	switch (lightType)
	{
	case LIGHT_TYPE_DIRECTIONAL:
		SetDirectionalLight(GL_LIGHT0, ldir[0], currentLC, ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright) ;
		break ;

	case LIGHT_TYPE_POINT:
		glPushMatrix() ;
		{
			glTranslatef(0.0f, 0.0f, B_DIST) ;
			SetPositionalLight(GL_LIGHT0, lpos[0], currentLC, ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN) ;
		}
		glPopMatrix() ;
		break ;

	case LIGHT_TYPE_SPOT:
		glPushMatrix() ;
		{
			glTranslatef(0.0f, 0.0f, B_DIST) ;
			SetPositionalLight(GL_LIGHT0, lpos[0], currentLC, ambLight * LOCAL_LIGHT_AMBIENT_RATIO, specularFlag, bright,
							   POSITIONAL_LIGHT_CONST_ATTN, POSITIONAL_LIGHT_LINEAR_ATTN, POSITIONAL_LIGHT_QUAD_ATTN,
							   sdir[0], SPOT_LIGHT_CUTOFF, SPOT_LIGHT_EXPONENT) ;
		}
		glPopMatrix() ;
		break ;
	}
*/

	// ͭ�������ƤΥ饤�Ȥο��򹹿�
	SetLightsColor() ;

	// ���ɴĶ������å�
	GLfloat	wamb[] = { ambLight * currentLC[0], ambLight * currentLC[1], ambLight * currentLC[2], 1.0f } ;
	// ���ɴĶ��� 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, wamb) ;
}


// �饤�ȤΥݥ�������Ϣ�Τߤ򹹿�
// ʿ�Ը������������ʤ� GL_POSITION �Τߡ�
// ���ݥåȥ饤�Ȥʤ� GL_POSITION ����� GL_SPOT_DIRECTION �򹹿�
void UpdateLightPosition(GLenum light, int element, float velocity)
{
	// �饤�Ȥθ����򹹿�
	for (int i = 0 ; i < 3 ; i ++)
	{
		lightAngle[element].v[i] += lightAngleVelocity[element].v[i] * velocity ;

		if (lightAngleVelocity[element].v[i] > 0.0f)
		{
			if (lightAngle[element].v[i] > 180.0f)
			lightAngle[element].v[i] -= 360.0f ;
		}
		else
		{
			if (lightAngle[element].v[i] < -180.0f)
			lightAngle[element].v[i] += 360.0f ;
		}
	}

	// OpenGL �˥��å�
	glPushMatrix() ;
	{
		glTranslatef(0.0f, 0.0f, B_DIST) ;

		glRotatef(lightAngle[element].X(), 1.0f, 0.0f, 0.0f) ;
		glRotatef(lightAngle[element].Y(), 0.0f, 1.0f, 0.0f) ;
		glRotatef(lightAngle[element].Z(), 0.0f, 0.0f, 1.0f) ;

		switch (lightType)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			glLightfv(light, GL_POSITION, ldir[element]) ;
			break ;
		case LIGHT_TYPE_POINT:
			glLightfv(light, GL_POSITION, lpos[element]) ;
			break ;
		case LIGHT_TYPE_SPOT:
			glLightfv(light, GL_POSITION, lpos[element]) ;
			glLightfv(light, GL_SPOT_DIRECTION, sdir[element]) ;
			break ;
		}
	}
	glPopMatrix() ;
}

// ���ĤΥ饤�ȤΥݥ�������Ϣ�Τߤ򹹿�
void UpdateLightsPosition()
{
//	glMatrixMode(GL_MODELVIEW) ;

	// �饤�ƥ��󥰤�ɬ�פ�����Τ�
	if (!vertexShadingFlag) return ;

	// FPS �ˤ�äƲ�ž®�٤��Ѳ�
	float velocity ;
	if (benchmark)
	{
		if (fps > 0.0f)
		{
			velocity = 480.0f / fps ;	// 60.0 FPS �� �ܡݣ���
			if (velocity > 32.0f)
				velocity = 32.0f ;
		}
		else
			velocity = 8.0f ;
	}
	else
	{
		if (fps > 0.0f)
		{
			velocity = 120.0f / fps ;	// 60.0 FPS �� �ܡݣ���
			if (velocity > 8.0f)
				velocity = 8.0f ;
		}
		else
			velocity = 3.0f ;
	}

	switch (light)
	{
	case 8:
		UpdateLightPosition(GL_LIGHT7, 7, velocity) ;
	case 7:
		UpdateLightPosition(GL_LIGHT6, 6, velocity) ;
	case 6:
		UpdateLightPosition(GL_LIGHT5, 5, velocity) ;

	case 5:
		UpdateLightPosition(GL_LIGHT4, 4, velocity) ;
	case 4:
		UpdateLightPosition(GL_LIGHT3, 3, velocity) ;
	case 3:
		UpdateLightPosition(GL_LIGHT2, 2, velocity) ;
	case 2:
		UpdateLightPosition(GL_LIGHT1, 1, velocity) ;
	case 1:
		UpdateLightPosition(GL_LIGHT0, 0, velocity) ;
	}
}


// ���׿ˤβ�ž
void ClockProc()
{
	float r_ratio ;

	timeData.t = time(NULL) ;
	timeData.t_area = localtime(&timeData.t) ;

	timeData.p_hour   = timeData.n_hour ;
	timeData.p_minute = timeData.n_minute ;
	timeData.p_second = timeData.n_second ;

	timeData.n_hour   = timeData.t_area->tm_hour % 12 ;
	timeData.n_minute = timeData.t_area->tm_min ;
	timeData.n_second = timeData.t_area->tm_sec ;

	// ���֤ˤ�äƻ��׿ˤγ��٤�׻�
	if (fps <= 1.0f) r_ratio = 1.0f ;
	else             r_ratio = 0.8f / fps + 0.2f ;

	if (timeData.n_hour   < timeData.p_hour  ) now.tm.hour   -= 360.0f ;
	if (timeData.n_minute < timeData.p_minute) now.tm.minute -= 360.0f ;
	if (timeData.n_second < timeData.p_second) now.tm.second -= 360.0f ;

	// ���ơ�������¸�ʿˤβ�ž���١�
	pre.tm = now.tm ;

	now.tm.hour   += ((timeData.n_hour   * 30 + timeData.n_minute * 0.5f) - now.tm.hour   ) * r_ratio ;
	now.tm.minute += ((timeData.n_minute *  6 + timeData.n_second * 0.1f) - now.tm.minute ) * r_ratio ;
	now.tm.second += ((timeData.n_second *  6                           ) - now.tm.second ) * r_ratio ;

	if (f_cnt >= F_COUNT) f_cal_flag = TRUE ;

	// ľ���Υ롼�פ�����֤��Ѥ�äƤ���ʤ�
	if (timeData.n_second != timeData.p_second ||
	    timeData.n_minute != timeData.p_minute ||
	    timeData.n_hour   != timeData.p_hour)
	{
		f_time = (timeData.n_hour   - timeData.fp_hour  ) * 3600 +
				 (timeData.n_minute - timeData.fp_minute) *   60 +
				 (timeData.n_second - timeData.fp_second) ;

		// ������֤�����롼�פ��᤮����FPS�׻�
		if (!benchmark && (f_cal_flag || f_time >= F_TIME_COUNT))
			FramesPerSecond() ;
	}

	// �٥���ޡ����ξ��ϰ��ž���
	if (benchmark)
	{
		if (spin_x != 0.0f)
		{
			if (f_cnt >= dst_fps_count)
//			if (f_cnt >= (-360.0f / spin_x) + 0.5f)
				FramesPerSecond() ;

			float axis ;
//			if (!(master_fps_count & 0x1))	// �ȥ��륹���å�
				axis = cos(Rad((f_cnt + 2) * spin_x * 0.5f)) ;
//			else
//				axis = cos(Rad((f_cnt + 2) * spin_x * 0.5f + 180.0f)) ;

			// �ե��륿�ƥ��Ȼ���
			// �����꡼�󥵥������ѹ�
			// axis (-1.0 �� 1.0)
			if (filterTest) // || maxTriTest)
			{
				// �ե��륿�ƥ��Ȼ���
				// �ƥ������������Ѳ�
				glMatrixMode(GL_TEXTURE) ;
				{
					axis = axis + 1.0f ;
					float s = 1.5f / (axis + 0.03f) / FILTER_TEST_TEX_SCALE ;

					glLoadIdentity() ;
					glTranslatef(0.5f, 0.5f, 0.0f) ;
					glScalef(s, s, s) ;
				}
				glMatrixMode(GL_MODELVIEW) ;
			}
			else if (maxTriTest)
			{
				screen_size = 0.02f ;

				// Maximum Triangles �ƥ��ȤǤϻ��פε�Υ���Ѳ�
				dist_y = (1.1f - axis) * B_DIST * 40.0f + B_DIST ;
			}
			else if (focusTest)
			{
				// Depth of Field �ƥ��ȤǤϡ�������Υ���Ѳ�
				focus_distance = axis * 30 - 50 ;	// -20��-80
			}
		}
	}
	else if (mode_solid && (filterTest || mipmapTest))
	{
		// !Benchmark �� filterTest or MipmapTest ����ƥ����������ʥ������ˤ��Ѳ�
//		float sx = now.pers, sy = dist_y / B_DIST ;
//		float sx = (float)mouse_x / xsize, sy = (float)mouse_y / ysize ;
		float sx = now.pers, sy = now.y_pers ;

		glMatrixMode(GL_TEXTURE) ;
		{
			glLoadIdentity() ;
			glTranslatef(0.5f, 0.5f, 0.0f) ;
			glScalef(sx, sy, 1.0f) ;
		}
		glMatrixMode(GL_MODELVIEW) ;
	}


	if (!bg_color)
	{
		if (!benchmark || (fps_count))// < 0 && testCount == 0))
			SetCurrentBackGroundColor() ;	// ���ߤλ��֤��顢�£ǥ��顼�����
//		else	// benchmark && fps_count >= 1
//			glFogfv(GL_FOG_COLOR, currentBG) ;
	}

	if (rotateLight)
		UpdateLightsPosition() ;

	// �ÿˤ����Υ⡼������ѥ������ȥ��󥰥뤪��ӥ��ƥå��ͷ׻�
	if (s_motion) CalcNeedleMotionBlurAngles(pre, now) ;

#ifdef GIFF_DEMO
	now.tm.hour   = GIFF_DEMO_HOUR   * 30 + GIFF_DEMO_MINUTE * 0.5f ;
	now.tm.minute = GIFF_DEMO_MINUTE *  6 + GIFF_DEMO_SECOND * 0.1f ;
	now.tm.second = GIFF_DEMO_SECOND *  6 ;
#endif
}

void CalculateMoving()
{
	if (DragR && (last_x != mouse_x || last_y != mouse_y))
	{
		// Drag to RIGHT MOUSE BUTTON

		// 0.0f��1.0f = 2.0f^3��2.0f^-3��8��1��1/8��
		p_step = pow(2.0f, (0.5f - ((float)mouse_x / xsize)) * 6.0f) ;
		if      (p_step < p_min) p_step = p_min ;
		else if (p_step > p_max) p_step = p_max ;

		p_step = (p_step - now.pers) * t_ratio ;

		yp_step = pow(2.0f, (0.5f - ((float)mouse_y / ysize)) * 6.0f) ;
		if      (yp_step < p_min) yp_step = p_min ;
		else if (yp_step > p_max) yp_step = p_max ;

		yp_step = (yp_step - now.y_pers) * t_ratio ;

		z_step = (((float)mouse_y / ysize) - 0.5f) * m_speed + B_DIST ;
		z_step = (z_step - dist_y) * t_ratio ;

		if      (z_step < -t_max) z_step = -t_max ;
		else if (z_step >  t_max) z_step =  t_max ;

		last_x = mouse_x ;
		last_y = mouse_y ;
	}
	else if ((DragL || DragL_t) && (mouse_x != last_x || mouse_y != last_y))
	{
		// Drag to LEFT MOUSE BUTTON

		spin_x = (mouse_x - last_x) * m_speed / xsize ;
		spin_y = (mouse_y - last_y) * m_speed / ysize ;

		last_x = mouse_x ;
		last_y = mouse_y ;

		DragL_t = OFF ;
	}

	// isSupported_glutKeyboardStatus only
/*
	if (keyL)
	{
		spin_x += ((-m_speed * 0.2) - spin_x) * 0.05 ;
	}
	if (keyR)
	{
		spin_x += ((m_speed * 0.2) - spin_x) * 0.05 ;
	}
	if (keyU)
	{
		spin_y += ((-m_speed * 0.2) - spin_y) * 0.05 ;
	}
	if (keyD)
	{
		spin_y += ((m_speed * 0.2) - spin_y) * 0.05 ;
	}
*/
	if (keyL)
	{
		spin_x += Sgn(((-m_speed * 0.08) - spin_x)) * m_speed * 0.004 ;
	}
	if (keyR)
	{
		spin_x += Sgn(((m_speed * 0.08) - spin_x)) * m_speed * 0.004 ;
	}
	if (keyU)
	{
		spin_y += Sgn(((-m_speed * 0.08) - spin_y)) * m_speed * 0.004 ;
	}
	if (keyD)
	{
		spin_y += Sgn(((m_speed * 0.08) - spin_y)) * m_speed * 0.004 ;
	}
}

// ����ư�����
void MoveModel()
{
	loop_count ++ ;	// ���פ�ư�������

	if (!benchmark)
		CalculateMoving() ;

	pre.pers    = now.pers ;
	pre.y_pers  = now.y_pers ;
	pre.clock_z = now.clock_z ;
	pre.roof    = now.roof ;

	now.roof += (t_roof - now.roof) * t_ratio ;	// ����ư��

	now.pers += p_step ;
	p_step *= i_ratio ;

	now.y_pers += yp_step ;
	yp_step *= i_ratio ;

	dist_y += z_step ;
	z_step *= i_ratio ;

#ifndef GIFF_DEMO
	if (!benchmark && !saver)
	{
		spin_x *= ir_ratio ;
		spin_y *= ir_ratio ;
	}
#endif

	now.clock_z = dist_y / now.pers * (now.roof + 2.0f) / 3.0f ;	// ���ףں�ɸ

	// ˾�󡢹��ѽ����ʥ���ƥ������ꥢ���󥰡��⡼�����֥�ʤ��λ���
	// ��̳����٤ϥӥ塼����Τߤǽ���
	if (!anti && !motion) SetPerspective(now) ;
	ClockProc() ;


	// �����꡼�󥻡��л���
	// �������Хåե��ȥӥ塼�ݡ��Ȱ��֤�����
	if (saver)
	{
//		spin_x = -cos(loop_count / 212.0f) * 3.5f ;
		spin_x = (-cos(loop_count / SPIN_RATIO * spinSpeed) + sin(loop_count / SPIN_RATIO * spinSpeed * 4.0f)) * 0.5 * SPIN_VELOC * spinSpeed ;

		// viewportX, viewportY ����ȥ�����ʤ������޻����͡�
		viewportX += viewportVX ;
		viewportY += viewportVY ;

		if (viewportX < 0)
		{
			viewportX = 0 ;
			viewportVX = -viewportVX ;
		}
		else if (viewportX + scissorSize >= is_set_width)
		{
			viewportX = is_set_width - scissorSize ;
			viewportVX = -viewportVX ;
		}

		if (viewportY < 0)
		{
			viewportY = 0 ;
			viewportVY = -viewportVY ;
		}
		else if (viewportY + scissorSize >= is_set_height)
		{
			viewportY = is_set_height - scissorSize ;
			viewportVY = -viewportVY ;
		}

//		viewportX = is_set_width  / 2 - scissorSize / 2 ;
//		viewportY = is_set_height / 2 - scissorSize / 2 ;

		// �������ϰϤȥӥ塼�ݡ��Ȥ�����
//		// 3Dfx OpenGL �� /* �������Ȥΰ������ϥ���������ʤ� */
//		if (!(fxOpenGL /* && start*/)) glScissor(viewportX, viewportY, scissorSize, scissorSize) ;


		if (loop_count < 5)
		{
			glDisable(GL_SCISSOR_TEST) ;
			glViewport(0, 0, is_set_width, is_set_height) ;
			glClear(GL_COLOR_BUFFER_BIT) ;
			glEnable(GL_SCISSOR_TEST) ;
		}


		glScissor(viewportX, viewportY, scissorSize, scissorSize) ;
		glViewport(viewportX + 2, viewportY + 2, scissorSize - 4, scissorSize - 4) ;
	}

//	glutPostRedisplay() ;	// ���ץ������
}


// ��ǥ����衢���󥻥åȡ���ž������
void DrawModelMatrix(ClockState& st)
{
	glPushMatrix() ;
	{
		glPushMatrix() ;
		{
			glLoadIdentity() ;

//			glRotatef(spin_x, 0.0f,1.0f,0.0f) ;	// ������ž
//			glRotatef(spin_y, 1.0f,0.0f,0.0f) ;	// ������ž

			if (spin_x != 0.0f || spin_y != 0.0f)
			{
				static Vector3d axis ;

				axis(spin_y, spin_x, 0.0f) ;
				axis.Unit() ;
				glRotatef(sqrt(spin_x * spin_x + spin_y * spin_y), axis.X(), axis.Y(), axis.Z()) ;
			}

			glMultMatrixf(aRotate) ;
			glGetFloatv(GL_MODELVIEW_MATRIX, aRotate) ;
		}
		glPopMatrix() ;

		if (benchmark || !mode_solid || (!filterTest && !mipmapTest))
			glTranslatef(0.0f, 0.0f, st.clock_z) ;	// ������ư
		else
			glTranslatef(0.0f, 0.0f, B_DIST) ;	// ������ư

		glMultMatrixf(aRotate) ;
		glTranslatef(0.0f, -st.roof * ROOF_MOVE, 0.0f) ;	// ������ư

		zAxis = aRotate[10] ;
		cull_status = CullingStatus(zAxis) ;

		if (stop)
		{
			if (start)
			{
				start = FALSE ;

				glNewList(AllocAList(CLOCK_LIST), GL_COMPILE_AND_EXECUTE) ;
					DrawClock(st) ;
				glEndList() ;
			}
			else
				glCallList(CLOCK_LIST) ;
		}
		else
		{
			DrawClock(st) ;
			if (start) start = FALSE ;
		}
	}
	glPopMatrix() ;
}


void InitLightsAngles()
{
	for (int l = 0 ; l < 8 ; l ++)
	{
		for (int i = 0 ; i < 3 ; i ++)
		{
			lightAngle[l].v[i] = 0.0f ;
			lightAngleVelocity[l].v[i] = ((float)rand() / RAND_MAX * 2.0 - 1.0) ;	// �ܡݣ���
		}

		Vector3d u(ldir[l]) ;
		u.Unit() ;
		ldir[l][0] = u.X() ;
		ldir[l][1] = u.Y() ;
		ldir[l][2] = u.Z() ;

		sdir[l][0] = -u.X() ;
		sdir[l][1] = -u.Y() ;
		sdir[l][2] = -u.Z() ;

		lpos[l][0] = ldir[l][0] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][1] = ldir[l][1] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][2] = ldir[l][2] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][3] = 1.0f ; // / POSITIONAL_LIGHT_W ;
	}
}


// ��ǥ����衢����ƥ������ꥢ���󥰡��⡼�����֥�ʥ��֡�
void DrawModel_DepthOfField()
{
	// �ǥץ����֥ե�����ɥ֥顼
	int i ;

	if (!crystal && texture != 1)
	{
		// �̾�Υ������졼�����Хåե���������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ				glAccum(GL_LOAD, 1.0f / field) ;
		DrawModelMatrixDepthOfField(now, 0) ;
		glAccum(GL_LOAD, 1.0f / field) ;

		for (i = 1 ; i < field ; i ++)
		{
			// 1/field ����
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
			DrawModelMatrixDepthOfField(now, i) ;
			glAccum(GL_ACCUM, 1.0f / field) ;
		}
		glAccum(GL_RETURN, 1.0f) ;
	}
	else if (crystal)
	{
		// ���ꥹ������פξ���ñ�ʤ�û�
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ				glAccum(GL_LOAD, 1.0f / field) ;

		for (i = 0 ; i < field ; i ++)
		{
			// 1/field ����
			DrawModelMatrixDepthOfField(now, i, FALSE) ;
		}
	}
	else
	{
		// �᥿�룱������åɣ��Ϧ��֥��ɤǽ���
		glDisable(GL_DITHER) ;

		glDisable(GL_BLEND) ;
		glDepthFunc(GL_LESS) ;
		glDepthMask(GL_TRUE) ;
		glEnable(GL_DEPTH_TEST) ;
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) ;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ

		// �����ܤ�����ϥ֥��� OFF �ǡ��̾�Ȥޤä���Ʊ������
		DrawModelMatrixDepthOfField(now, 0, FALSE) ;
//		DrawModelMatrixDepthOfField(now, 0, TRUE) ;
//		DrawModelMatrix(now) ;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		glEnable(GL_BLEND) ;
		glDepthFunc(GL_LEQUAL) ;

		for (i = 1 ; i < field ; i ++)
		{
			// 1/field ����
			DrawModelMatrixDepthOfField(now, i, TRUE) ;
		}

		glDepthFunc(GL_LESS) ;
		glDisable(GL_BLEND) ;

		if (dither) glEnable(GL_DITHER) ;
	}
}


void DrawModel_WithoutAntiAlias()
{
	if (!motion)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
//		glClear(GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
//		ClearViewPortWithRect() ;

		DrawModelMatrix(now) ;
	}
	else
	{
		int i ;

		if (!crystal && texture != 1)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
			if (shutter != 1.0f || !pre_motion)
			{
				pre_motion = motion ;	// ľ���� motion ��¸

				DrawModelMatrix(pre, now, 0) ;
				glAccum(GL_LOAD, 1.0f / motion) ;
			}
			else
				glAccum(GL_MULT, 1.0f / motion) ;

			for (i = 1 ; i < motion ; i ++)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
				DrawModelMatrix(pre, now, i) ;
				glAccum(GL_ACCUM, 1.0f / motion) ;
			}
			glAccum(GL_RETURN, 1.0f) ;
		}
		else if (crystal)
		{	// ���ꥹ������פξ���ñ�ʤ�û����ɤ��Τǥ������졼�����Хåե��ϻȤ�ʤ�
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
			for (i = 0 ; i < motion ; i ++)
				DrawModelMatrix(pre, now, i) ;
		}
		else
		{	// �������졼�����Хåե��ϻȤ�ʤ�
			glDisable(GL_DITHER) ;

			if (shutter != 1.0f || !pre_motion)
			{
				pre_motion = motion ;	// ľ���� motion ��¸

				glDisable(GL_BLEND) ;
				glDepthFunc(GL_LESS) ;
				glDepthMask(GL_TRUE) ;
				glEnable(GL_DEPTH_TEST) ;

				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) ;
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ

				// �����ܤ�����ϥ֥��� OFF �ǡ��̾�Ȥޤä���Ʊ������
				DrawModelMatrixWithOverSampling(pre, now, 0) ;
			}
			else if (!DONOT_USE_GL_READ_BUFFER && glut_buff == GLUT_DOUBLE)
			{
				// ���󤫤�������Ĥ��ʻ��������Ρ˻������֥�Хåե��¹Ԥʤ�
				// ����ΥХå��Хåե��ʸ���ɽ������Ƥ������ơˤ�
				// ���ߤΥɥ����Хåե��˥��ԡ�

				// �ɤ߽Ф��٤��Хåե��ʥɥ����Хåե��εաˤ� ���å�
				GLint draw_buffer ;
				glGetIntegerv(GL_DRAW_BUFFER, &draw_buffer) ;
				if (draw_buffer == GL_FRONT)
					draw_buffer = GL_BACK ;
				else
					draw_buffer = GL_FRONT ;

				glReadBuffer((GLenum)draw_buffer) ;

				// �饹���ݥ�����������ȥХåե��Υ��ԡ�
				glPushMatrix() ;
				glLoadIdentity() ;

				glMatrixMode(GL_PROJECTION) ;
				glPushMatrix() ;
				glLoadIdentity() ;

				glDisable(GL_DEPTH_TEST) ;
				glRasterPos3f(-1.0f, -1.0f, 0.0f) ;
				glCopyPixels(0,0, xsize,ysize, GL_COLOR) ;
				glEnable(GL_DEPTH_TEST) ;

				glPopMatrix() ;

				glMatrixMode(GL_MODELVIEW) ;
				glPopMatrix() ;
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			glEnable(GL_BLEND) ;
			glDepthFunc(GL_LEQUAL) ;

			for (i = 1 ; i < motion ; i ++)
				DrawModelMatrixWithOverSampling(pre, now, i) ;

			glDepthFunc(GL_LESS) ;
			glDisable(GL_BLEND) ;

			if (dither) glEnable(GL_DITHER) ;
		}
	}
}


void DrawModel_WithoutDOFAA()
{
	if (!motion)
	{
		int i ;

		if (!crystal && texture != 1)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ				glAccum(GL_LOAD, 1.0f / anti) ;
			SetPerspective(now, jitter[0].x, jitter[0].y) ;
			DrawModelMatrix(now) ;
			glAccum(GL_LOAD, 1.0f / anti) ;

			for (i = 1 ; i < anti ; i ++)
			{
				// Ʃ��ͱƹ��󥻥å�
				Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, jitter[i].x,jitter[i].y) ;

				// 1/anti ����
				glMatrixMode(GL_MODELVIEW) ;							// ����⡼�ɤ��ǥ�ӥ塼�˥��å�
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
				DrawModelMatrixAnti(now) ;
				glAccum(GL_ACCUM, 1.0f / anti) ;
			}
			glAccum(GL_RETURN, 1.0f) ;
		}
		else if (crystal)
		{
			// ���ꥹ������פξ���ñ�ʤ�û�
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ				glAccum(GL_LOAD, 1.0f / anti) ;
			SetPerspective(now, jitter[0].x, jitter[0].y) ;
			DrawModelMatrix(now) ;

			for (i = 1 ; i < anti ; i ++)
			{
				// Ʃ��ͱƹ��󥻥å�
				Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, jitter[i].x,jitter[i].y) ;

				// 1/anti ����
				glMatrixMode(GL_MODELVIEW) ;			// ����⡼�ɤ��ǥ�ӥ塼�˥��å�
				DrawModelMatrixAnti(now) ;
			}
		}
		else
		{
			glDisable(GL_DITHER) ;

			glDisable(GL_BLEND) ;
			glDepthFunc(GL_LESS) ;
			glDepthMask(GL_TRUE) ;
			glEnable(GL_DEPTH_TEST) ;
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) ;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ

			// �����ܤ�����ϥ֥��� OFF �ǡ��̾�Ȥޤä���Ʊ������
			SetPerspective(now, jitter[0].x, jitter[0].y) ;
//			DrawModelMatrixAntiWithOverSampling(now, 0) ;
			DrawModelMatrix(now) ;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			glEnable(GL_BLEND) ;
			glDepthFunc(GL_LEQUAL) ;

			for (i = 1 ; i < anti ; i ++)
			{
				// Ʃ��ͱƹ��󥻥å�
				Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, jitter[i].x,jitter[i].y) ;

				// 1/anti ����
				glMatrixMode(GL_MODELVIEW) ;			// ����⡼�ɤ��ǥ�ӥ塼�˥��å�
				DrawModelMatrixAntiWithOverSampling(now, i) ;
			}

			glDepthFunc(GL_LESS) ;
			glDisable(GL_BLEND) ;

			if (dither) glEnable(GL_DITHER) ;
		}
	}
	else
	{
		int        i ;
		ClockState tmp ;

		for (i = 0 ; i < motion ; i ++)
		{
			// 1/motion ����
			int j ;

			SetPerspective(now, jitter[0].x, jitter[0].y) ;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
			DrawModelMatrix(pre, now, i, &tmp) ;

			if (!i) glAccum(GL_LOAD,  1.0f / motion / anti) ;
			else    glAccum(GL_ACCUM, 1.0f / motion / anti) ;

//			if (!crystal)
			{
				for (j = 1 ; j < anti ; j ++)
				{
					// Ʃ��ͱƹ��󥻥å�
					Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, jitter[j].x,jitter[j].y) ;

					// 1/anti/motion ����
					glMatrixMode(GL_MODELVIEW) ;							// ����⡼�ɤ��ǥ�ӥ塼�˥��å�
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;	// ���顼���ǥץ��Хåե����ꥢ
					DrawModelMatrixAnti(tmp) ;
					glAccum(GL_ACCUM, 1.0f / motion / anti) ;
				}
				glAccum(GL_RETURN, 1.0f) ;
			}
/*			else
			{	// ���ꥹ������פξ���ñ�ʤ�û����ɤ��Τǥ������졼�����Хåե��ϻȤ�ʤ�
				for (j = 0 ; j < anti ; j ++)
				{
					// Ʃ��ͱƹ��󥻥å�
					Perspective(scr_w,scr_h, near_clip,far_clip, xsize,ysize, jitter[j].x,jitter[j].y) ;

					// 1/anti/motion ����
					glMatrixMode(GL_MODELVIEW) ;							// ����⡼�ɤ��ǥ�ӥ塼�˥��å�
					DrawModelMatrixAnti(tmp) ;
				}
			}
*/
		}
	}
}


// ��ǥ����衢����ƥ������ꥢ���󥰡��⡼�����֥�
void DrawModel()
{
	CheckOpenGLError() ;
	if (saver && firstReshapeTime == -1) return ;

	// Mesa???
	// �ޤä�����������
	// Mesa �Ǥϡ����Τ��饤�Ȥ��Ť��ʤäƤ��ޤ�
	if (mode_normal)
	{
		static int tmp ;
		if (loop_count == 2)
		{
			tmp = lightType ;
			lightType = LIGHT_TYPE_SPOT ;
		}
		else if (loop_count == 3)
		{
			SetLightModel(FALSE) ;
			lightType = tmp ;
			SetLightModel(FALSE) ;
		}
	}


	// ���פ�ư��ʲ�ž����ư����
	MoveModel() ;

	if (field)
	{
		// �ǥץ����֥ե�����ɥ֥顼
		DrawModel_DepthOfField() ;
	}
	else if (!anti)
	{
		// !anti
		DrawModel_WithoutAntiAlias() ;
	}
	else
	{
		// !field && !anti
		DrawModel_WithoutDOFAA() ;
	}

	// ���������褢�� �ޤ��� FPS ɽ�� ON �ޤ��� ��������ɽ������ǥɥ�å���ʤ�
	if ((!no_sign || f_prn || (drawCursor && (DragL || DragR))) && loop_count >= 10)
		DrawSignAndFPS() ;	// �����󤪤�ӣƣУӡ�������������

	if (glut_buff == GLUT_DOUBLE) glutSwapBuffers() ;	// �ɥ����Хåե��ȥӥ塼�Хåե��Υ���å�
	else glFlush() ;	// ���󥰥�Хååե��⡼�ɻ��ϥꥯ�����ȥХåե���ե�å���

	if (us_sleep) u_sleep(us_sleep) ;

	f_cnt ++ ;		// �롼�ײ���������


//	logprintf("%f\n", spin_x, spin_y) ;


	// ���褷���ݥꥴ���������� && �����Х���ץ�����褷���ºݤΥݥꥴ����׻�

	// �ÿ˥⡼�����֥顼
	if (s_motion && !motion)
		r_polygons = nm_polygons + 48 * (s_motion - 1) ;
	else
		r_polygons = nm_polygons ;

	if (anti)
	{
		if (motion)
			r_polygons *= motion * anti ;
		else
		{
			// anti �Τߤǡ�-e 1, -s 1 �ξ��ʼºݤ�������ϥ���ץ�󥰲�������ݣ���
			if (mode_env == 1 || mode_solid == 1)	// || mode_crystal)
				r_polygons = r_polygons * (anti * 2 - 1) + (anti - 1) * 2 ;
			else
				r_polygons *= anti ;
		}
	}
	else if (motion)
	{
		// motion �Τߤǡ�-e 1, -s 1, -C �ξ��ʼºݤ�������ϥ���ץ�󥰲�������ݣ���
		if (mode_env == 1 || mode_solid == 1)	// || mode_crystal)
//			r_polygons *= motion * 2 - 1 ;
			r_polygons = r_polygons * (motion * 2 - 1) + (motion - 1) * 2 ;
		else
			r_polygons *= motion ;
	}
	else if (field)
	{
		// field �ǡ�-e 1, -s 1, -C �ξ��ʼºݤ�������ϥ���ץ�󥰲�������ݣ���
		if (mode_env == 1 || mode_solid == 1)	// || mode_crystal)
//			r_polygons *= field * 2 - 1 ;
			r_polygons = r_polygons * (field * 2 - 1) + (field - 1) * 2 ;
		else
			r_polygons *= field ;
	}

	f_polygons += r_polygons ;


	// ����Υ��ơ�������¸�ʥ��饹��¿���Τβ�ž���١�
	pre.g_spin   = now.g_spin ;
	pre.h_rotate = now.h_rotate ;

	now.g_spin += 1.0f ;		// ɽ�̥��饹�β�ž
	now.h_rotate += 5.0f ;	// ¿���Τβ�ž
}

void Redisplay()
{
	glutPostRedisplay() ;
}

// �ޥ�����ư������Хå�
void MouseMotion(int x, int y)
{
	mouse_x = x ;
	mouse_y = y ;
}

// �ɥ�å� �ϣΡ��ϣƣ�
void MouseProc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// �ɥ�å��ϣλ��ϥޥ����Υɥ�å����ΰ��֤˸��ߤΰ��֤���¸
		if      (state == GLUT_DOWN) { DragL = ON  ; last_x = x ; last_y = y ; }
		else if (state == GLUT_UP  ) { DragL = OFF ; DragL_t = ON ; }
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		// �ɥ�å��ϣλ��ϥޥ����Υɥ�å����ΰ��֤˸��ߤΰ��֤���¸
		if      (state == GLUT_DOWN) { DragR = ON  ; last_x = x ; last_y = y ; }
		else if (state == GLUT_UP  ) { DragR = OFF ; }
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		t_roof = 1.0f - t_roof ;	// ���ڡ����ǳ�����
		last_x = x ; last_y = y ;
	}

	mouse_x = x ;
	mouse_y = y ;
}


void SpecialKeyProc(int key, int x, int y)
{
	if (isSupported_glutKeyboardStatus)
	{
		if      (key == GLUT_KEY_LEFT ) keyL = ON ;
		else if (key == GLUT_KEY_RIGHT) keyR = ON ;
		else if (key == GLUT_KEY_UP   ) keyU = ON ;
		else if (key == GLUT_KEY_DOWN ) keyD = ON ;
	}
	else
	{
		if      (key == GLUT_KEY_LEFT ) spin_x += ((-0.2f * SPEED) - spin_x) * 0.2f ;
		else if (key == GLUT_KEY_RIGHT) spin_x += (( 0.2f * SPEED) - spin_x) * 0.2f ;
		else if (key == GLUT_KEY_UP   ) spin_y += ((-0.2f * SPEED) - spin_y) * 0.2f ;
		else if (key == GLUT_KEY_DOWN ) spin_y += (( 0.2f * SPEED) - spin_y) * 0.2f ;
	}

	last_x = mouse_x = x ;
	last_y = mouse_y = y ;
}

// isSupported_glutKeyboardStatus only
void SpecialKeyUpProc(int key, int x, int y)
{
	if      (key == GLUT_KEY_LEFT )
	{
		keyL = OFF ;
		if (spin_x < 0.0) spin_x *= 0.05 ;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		keyR = OFF ;
		if (spin_x > 0.0) spin_x *= 0.05 ;
	}
	else if (key == GLUT_KEY_UP   )
	{
		keyU = OFF ;
		if (spin_y < 0.0) spin_y *= 0.05 ;
	}
	else if (key == GLUT_KEY_DOWN )
	{
		keyD = OFF ;
		if (spin_y > 0.0) spin_y *= 0.05 ;
	}

	mouse_x = x ;
	mouse_y = y ;
}


// ������ɥ���������xsize, ysize�ˤˤ�äƥ�����ΰ��֡�sign_x, sign_y�ˤ�׻�
void SetSignPosition()
{
	sign_x = (int)(xsize * 0.97f - 5.0f * sign.Length()) ;
	sign_y = (int)(ysize * 0.03f + 2.0f) ;
}

// ������ɥ���������xsize, ysize�ˤˤ�ä�FPS�ΰ��֡�fps_x, fps_y�ˤ�׻�
void SetFPSPosition()
{
	fps_x = (int)(xsize * 0.03f +  2.0f) ;
	fps_y = (int)(ysize * 0.97f - 12.0f) ;
}


// �����꡼�󥻡��е�ư�������餫�Υ�������󤬤��ä����

// ������
void ExitSaver()
{
	EndProc() ;

#ifdef WIN32
	// IME �и�
	if (pWINNLSEnableIME)
		WINNLSEnableIME(NULL, TRUE) ;

	// DLL ����
	if (hLibUser32)
		FreeLibrary(hLibUser32) ;

#endif

	glclock_exit(EXIT_SUCCESS) ;
}

// glutPassiveMotionFunc
void ExitSaverWait(int x, int y)
{
	static int preX = -99999, preY ;

	// ���̤��и����Ƥ��飱�ð���Ͻ�λ���ʤ�
//	if (glutGet((GLenum)GLUT_ELAPSED_TIME) < firstReshapeTime + 1000 ||
//		firstReshapeTime == -1) return ;

	// �ǽ�Υ��٥�ȤǤϤʤ������Υ��٥�Ȼ���ꣵ�ɥåȰʾ�ư���Ƥ���н�λ
//	if (preX != -99999 &&
//		(Abs(x - preX) > 0 || Abs(y - preY) > 0))
	if (preX != -99999 &&
		(Abs(x - preX) > 4 || Abs(y - preY) > 4))
		ExitSaver() ;

	preX = x ;
	preY = y ;
}

// glutMotionFunc
void ExitSaver(int x, int y)
{
	ExitSaver() ;
}

// glutKeyboardFunc
void ExitSaver(unsigned char key, int x, int y)
{
	ExitSaver() ;
}

// glutMouseFunc
void ExitSaver(int key_button, int state, int x, int y)
{
	ExitSaver() ;
}

// glutSpecialFunc
void ExitSaver(int key, int x, int y)
{
	ExitSaver() ;
}


// ������ɥ��Υ���ե�����졼����󥤥٥�ȥϥ�ɥ�
void ReshapeWindow(int x, int y)
{
//	if (saver && firstReshapeTime == -1)
//		firstReshapeTime = glutGet((GLenum)GLUT_ELAPSED_TIME) ;

	int screen_y = y ;

#ifdef WIN32
	// Windows �Ǥϡ�glclock ������ɥ��Υϥ�ɥ����¸
	hWin = FindWindow("GLUT", title) ;
	if (hWin)
	{
		// Voodoo �⡼�ɤޤ��ϥ����꡼�󥻡��Х⡼�ɤǤ�
		// ��˼�����ɽ��
		if (saver)
//		if (voodoo || saver)
		{
/*
			WINDOWPLACEMENT wndpl ;
			wndpl.length = sizeof(WINDOWPLACEMENT) ;
			GetWindowPlacement(win, &wndpl) ;

			SetWindowPos(hWin, HWND_TOPMOST,
						 wndpl.ptMaxPosition.x,
						 wndpl.ptMaxPosition.y,
						 GetSystemMetrics(SM_CXSCREEN),
						 GetSystemMetrics(SM_CYSCREEN),
						 SWP_NOMOVE | SWP_NOSIZE) ;
*/
			SetWindowPos(hWin, HWND_TOPMOST,
						 0,0, 0,0,
						 SWP_NOMOVE | SWP_NOSIZE) ;
		}

		if (firstReshapeTime == -1 && !hIcon)
		{
//			hIcon = LoadIcon(NULL, MAKEINTRESOURCE(100)) ;
			hIcon = LoadImage(hInstanceGlClock, MAKEINTRESOURCE(ID_ICON_RESOURCE + iconNo), IMAGE_ICON, 0,0, LR_DEFAULTCOLOR) ;
			if (hIcon)
				SendMessage(hWin, WM_SETICON, TRUE, (LPARAM)hIcon) ;
		}
	}
#endif

	if (saver)
	{
		if (firstReshapeTime == -1)
//			firstReshapeTime = glutGet((GLenum)GLUT_ELAPSED_TIME) ;
			firstReshapeTime = GetMiliSeconds() ;

		// �����꡼�󥻡��Х⡼�ɤǤ� scissorSize ��
		// ���٥��顼�Хåե����Τ򥯥ꥢ
		is_set_width  = x ;
		is_set_height = y ;

		scissorSize = (int)(is_set_height * Pow(2.0, clockSize * 3.0) / 7.9999) ;
		if (scissorSize > is_set_height)
			scissorSize = is_set_height ;
		else if (scissorSize < 5)
			scissorSize = 5 ;

		srand(timeData.p_hour * timeData.p_minute * timeData.p_second) ;
		viewportX  = (int)((float)rand() / RAND_MAX * (is_set_width  - scissorSize)) ;
		viewportY  = (int)((float)rand() / RAND_MAX * (is_set_height - scissorSize)) ;
		viewportVX = (int)(Sgn((float)rand() / RAND_MAX - 0.5)) ;
		viewportVY = (int)(Sgn((float)rand() / RAND_MAX - 0.5)) ;

		glScissor(0, 0, is_set_width, is_set_height) ;
		glViewport(0, 0, is_set_width, is_set_height) ;

		glClear(GL_COLOR_BUFFER_BIT) ;

		// ���֥�Хåե�����
		// ����ɽ����ΥХåե��⥯�ꥢ���Ƥ���
		if (glut_buff == GLUT_DOUBLE)
		{
			glutSwapBuffers() ;
			glClear(GL_COLOR_BUFFER_BIT) ;
		}

		x = scissorSize ;
		y = scissorSize ;

//		glEnable(GL_SCISSOR_TEST) ;
	}
	else if (voodoo)
	{
		if (is_set_width ) x = is_set_width ;
		if (is_set_height) y = is_set_height ;
		glViewport(0, 0, x, y) ;
	}
	else
	{
		if (full && is_set_width && is_set_height)
		{
			x = is_set_width ;
			y = is_set_height ;
		}
		glViewport(0, screen_y - y, x, y) ;
	}

	glMatrixMode(GL_MODELVIEW) ;
	// Reset Matrix
	glLoadIdentity() ;
	gluLookAt(0.0f,0.0f,0.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f) ;

	// Save Now Window size for Mouse evnet
	xsize = x ;
	ysize = y ;

	if (x < (sign.Length() + 2) * 5 || y < 16) sign_x = NONE ;
	else SetSignPosition() ;
	if (x < 100) fps_x = NONE ;
	else SetFPSPosition() ;

	// shutter == 1.0f �ǥ⡼�������ϡ������ܤ����褫����ľ���ʥ������졼�����Хåե����Ȥ��ʤ������
	if (motion && shutter == 1.0f) pre_motion = FALSE ;


	// ɽ�������ޤޥꥵ�����Ǥ��륦����ɥ��ޥ͡�����Ǥϥ��٥�Ȥ�Ϣ³����ȯ�����뤿��ߤ᤿�����ɤ���
//	logprintf("Window size: %d x %d\n", x, y) ;
}

// ���פΥ�����ƥ����å�
void SetQuality(int q = QUALITY)
{
	if      (q < 1) q = 1 ;
	else if (q > 8) q = 8 ;

	// TNT �Ǥϡ�58 �ѷ���Ķ���ʤ��褦��
	if (envOpenGL == RIVA_TNT_FIX)
		quality[6][1] = quality[7][1] = 58 ;

	// ɸ����פޤ��� -s 2 �Ǥ� -q 5 �ʾ�Υ��饹�⤹�٤ƣ���
	if (solid || (!reality && !env && !crystal))
	{
		for (int i = 4 ; i < 8 ; i ++)
			quality[i][2] = 1 ;
	}

	// Filter Test
	if (!benchmark && solid == 1 && (filterTest || mipmapTest))
	{
		for (int q = 0 ; q < 8 ; q ++)
			quality[q][2] = 0 ;
	}

	clock_hedron = quality[q - 1][0] ;
	clock_div    = quality[q - 1][1] ;
	clock_glass  = quality[q - 1][2] ;
	clock_point  = quality[q - 1][3] ;
	clock_mesh   = quality[q - 1][4] ;
	gold_depth   = quality[q - 1][5] ;

/*
	// �Ρ��ޥ� �ޤ��� -solid 2, 3 �ʳ��λ��ס�-s 1, -e, -r, -C�ˤ�
	// ���פ�ϻ�ѷ��ʲ��ξ��� -glass 1 �ϻ����Բ�
	if (clock_glass == 1 &&
		((mode_normal && !light) ||
		  mode_solid == 1 ||
		  mode_env ||
		  mode_real ||
		  mode_crystal ||
		  clock_div <= 6)
		)
		clock_glass = 2 ;
*/
}


void InitVarEnvOpenGL(Option& option)
{
	int gl, setEnvFlag = FALSE ;

	gl = option.Get("-MESA", 5) ;
//#ifdef MESA
//	envOpenGL = MESA_LIB ; setEnvFlag = TRUE ; // Mesa �饤�֥��ǥ���ѥ������ɬ�� Mesa �⡼�ɤ�
//#else
	if (gl) { envOpenGL = MESA_LIB ; setEnvFlag = TRUE ; }
//#endif

	interrupt_OpenGLError = option.Get("-INT_GL_ERROR", 11) ;
	donotCheck_OpenGLError = option.Get("-NO_GL_ERROR", 10) ;

	donotUse_AutoSelectOpenGLEnvironmentMode = option.Get("-NO_AUTOSELECTGLMODE", 8) ;

	gl = option.Get("-3DFXMESA", 9) ;
	if (gl) { envOpenGL = VOODOO_MESA ; setEnvFlag = TRUE ; WIDTH = 640 ; HEIGHT = 480 ; }

	gl = option.Get("-3DFXMESA_OLD", 13) ;
	if (gl) { envOpenGL = VOODOO_MESA_OLD ; setEnvFlag = TRUE ; WIDTH = 640 ; HEIGHT = 480 ; }

	gl = option.Get("-VOODOO_AUTO", 12) ;
	if (gl) { envOpenGL = VOODOO_AUTO ; setEnvFlag = TRUE ; WIDTH = 640 ; HEIGHT = 480 ; }

	gl = option.Get("-VOODOO", 7) ;
	if (gl) { envOpenGL = VOODOO ; setEnvFlag = TRUE ; WIDTH = 640 ; HEIGHT = 480 ; }

	gl = option.Get("-3DFXICD", 8) ;
	if (gl) { envOpenGL = VOODOO_3DFX_ICD ; setEnvFlag = TRUE ; }

	gl = option.Get("-RUSH", 5) ;
	if (gl) { envOpenGL = VOODOO_RUSH ; setEnvFlag = TRUE ; }

//	gl = option.Get("-BANSHEE", 8) ;
//	if (gl) { envOpenGL = VOODOO_BANSHEE ; setEnvFlag = TRUE ; }

	gl = option.Get("-RIVA128", 5) ;
	if (gl) { envOpenGL = RIVA ; setEnvFlag = TRUE ; }

	gl = option.Get("-PERMEDIA2", 10) ;
	if (gl) { envOpenGL = PERMEDIA2 ; setEnvFlag = TRUE ; }

	gl = option.Get("-SAVAGE2000", 11) ;
	if (gl) { envOpenGL = SAVAGE2000 ; setEnvFlag = TRUE ; }

	gl = option.Get("-TNTFIX", 4) ;
	if (gl) { envOpenGL = RIVA_TNT_FIX ; setEnvFlag = TRUE ; }

	gl = option.Get("-G400", 5) ;
	if (gl) { envOpenGL = G400 ; setEnvFlag = TRUE ; }

	gl = option.Get("-GENERIC", 8) ;
	if (gl) { envOpenGL = GENERIC ; setEnvFlag = TRUE ; }

	gl = option.Get("-WIN95GDIFIX", 6) ;
	if (gl) { envOpenGL = WIN95_GDI_FIX ; setEnvFlag = TRUE ; }


	// ���ץ�������Τʤ��ǥե����
	// VOODOO_MODE �ʤ� VOODOO
	// ����ʳ��ʤ� GENERIC
	if (!setEnvFlag)
	{
#ifdef VOODOO_MODE
		envOpenGL = VOODOO ;
#else
		envOpenGL = GENERIC ;
#endif
	}

	if (envOpenGL == MESA_LIB || envOpenGL == VOODOO_MESA || envOpenGL == VOODOO_MESA_OLD) mesa = TRUE ;
	if (envOpenGL == VOODOO_AUTO || envOpenGL == VOODOO_MESA || envOpenGL == VOODOO_MESA_OLD || envOpenGL == VOODOO) voodoo = TRUE ;

	// PERMEDIA2, RIVA �ϡ�������������Ѥ��ʤ�
	if (envOpenGL == PERMEDIA2 || envOpenGL == RIVA) donotUSe_AlphaGouraud = TRUE ;
	else donotUSe_AlphaGouraud = option.Get("-NOAGOURAUD", 4) ;

	// 3Dfx OpenGL �Ǥϡ�glReadBuffer ��Ȥ��ʤ�
	if (envOpenGL == VOODOO || envOpenGL == VOODOO_RUSH ||
		envOpenGL == VOODOO_BANSHEE)
	{
		DONOT_USE_GL_READ_BUFFER = TRUE ;
		fxOpenGL = TRUE ;
	}

	// RIVA TNT �ǣڥХåե��˥Х����Ф���ޤ��� 3dfx Client Driver(Q3)
	if (envOpenGL == RIVA_TNT_FIX || envOpenGL == VOODOO_3DFX_ICD) zOffset = Z_OFFSET_TNT_FIX ;
	else                                                           zOffset = Z_OFFSET ;
}


void InitVarWindow(Option& option)
{
	// �����꡼�󥻡��е�ư���ץ����
	saver = option.Get("-SAVER", 3) ;

	if (!saver)
		no_message = option.Get("-NOMESG", 4) ;
	else
		no_message = TRUE ;

	// �����ե��������
	if (!no_message)
		logFile = option.GetString("-log", OUTPUT_LOG_FILE, 4) ;
	else
		logFile = "" ;

	if (logFile == "")	// OUTPUT_LOG_FILE)
	{
		// �����ν����褬���ꤵ��ʤ��ä�����ɸ����Ϥ�
		outstream = stdout ;
	}
	else
	{
		// �����ե����뤬���ꤵ�줿���
		if (!(outstream = fopen(logFile, "w")))
		{
#ifdef WIN32
			MessageBox(NULL, "Can't open log file: " + logFile, "File Error", MB_OK | MB_ICONSTOP) ;
#endif
			errprintf(String("Can't open log file: \"") + logFile + "\"\n") ;
			EndProc() ;
			glclock_exit(EXIT_FAILURE) ;
		}
	}

	// ������ɥ�������
	int isSetVGA = FALSE ;
	if ((isSetVGA = option.Get("-VGA", 4)))
	{
		xsize = is_set_width  = 640 ;
		ysize = is_set_height = 480 ;
	}
	else if ((isSetVGA = option.Get("-SVGA", 5)))
	{
		xsize = is_set_width  = 800 ;
		ysize = is_set_height = 600 ;
	}
	else if ((isSetVGA = option.Get("-XGA", 4)))
	{
		xsize = is_set_width  = 1024 ;
		ysize = is_set_height =  768 ;
	}
	else
	{
		xsize = is_set_width  = option.GetInt ("-width",  IS_SET_WIDTH,  2) ;
		ysize = is_set_height = option.GetInt ("-height", IS_SET_HEIGHT, 2) ;
	}

	if (!xsize) xsize = WIDTH ;
	if (!ysize) ysize = HEIGHT ;

	if (isSetVGA)
	{
		if (saver)
			full = FALSE ;
		else
			full = TRUE ;
	}
	else
		full = option.Get("-FULL", 3) ;

	if (voodoo)
	{
		// Voodoo �⡼�ɤǲ�����ꤵ��ʤ��ä�����
		// -w 640 -h 480 �����åȤ��줿���Ȥˤ���
		if (!is_set_width)  is_set_width  = WIDTH ;
		if (!is_set_height) is_set_height = HEIGHT ;

		if (saver)
			full = FALSE ;
		else
			full = TRUE ;
	}

#ifdef WIN32
	bpp = option.GetInt("-bppixel", BPPIXEL, 4) ;
#endif

	if (saver)
	{
		full = !full ;	// �����꡼�󥻡��е�ư���ϡ�-FU �������꡼��⡼��

		title = "glclock screen saver" ;

		// ���顼�����å���̵��
		donotCheck_OpenGLError = TRUE ;

#ifdef WIN32
		// IME �õ�

		// USER32.dll ���� WINNLSEnableIME ����
		// �ؿ����ɥ쥹�򥲥åȤǤ������ IME ��õ�
		hLibUser32 = LoadLibrary(_T(USER32_DLL_NAME)) ;
		if (hLibUser32)
		{
			// DLL �����ɤ�����������
			// WINNLSEnableIME �� ����ȥ�ݥ���Ȥ����
			pWINNLSEnableIME = (PFNWINNLSENABLEIMEPROC)GetProcAddress(hLibUser32, _T("WINNLSEnableIME")) ;
//			if (!pWINNLSEnableIME)
//				MessageBox(NULL, _T("Failed to get WINNLSEnableIME entry point"), _T("GetProcAddress Error"), MB_OK | MB_ICONSTOP) ;
		}
		else
		{
			// DLL �Υ����ɤ˼���
			pWINNLSEnableIME = NULL ;
//			MessageBox(NULL, _T("Failed to load USER32.DLL"), _T("LoadLibrary Error"), MB_OK | MB_ICONSTOP) ;
		}

		if (pWINNLSEnableIME)
			WINNLSEnableIME(NULL, FALSE) ;
#endif

		// �����꡼�󥻡��л������襵�����ʥ�������������
		// �����꡼�󥻡��аʳ��Υ⡼�ɤǤ� 1.0f �Ȥʤ�
		clockSize = option.GetDouble("-csize", CLOCK_SIZE, 3) ;
		if (clockSize > 1.0f) clockSize = 1.0f ;
	}

#ifndef IS_SUPPORTED_GLUT_FULL_SCREEN
	full = FALSE ;
#endif	// #ifndef IS_SUPPORTED_GLUT_FULL_SCREEN


	SetSignPosition() ;
}


void InitVarRendering(Option& option)
{
	m_speed    = option.GetDouble("-mspeed",    SPEED,     3) ;
	f_prn      = option.Get      ("-FPS",                  2) ;

	multi_super_sample = option.Get("-MULTISAMPLE",        6) ;	// Ʊ���������ĥ��ץ����
	anti       = option.GetInt   ("-anti",     ANTI,       2) ;

	field      = option.GetInt   ("-field",    FIELD,      2) ;
	dof_range  = option.GetDouble("-frange",   DEPTH_OF_FIELD_RANGE, 3) ;
	focus_distance = option.GetDouble("-focus", B_DIST,    6) ;
	focusTest  = option.Get      ("-FOCUSTEST",            6) ;

	motion     = option.GetInt   ("-motion",   MOTION,     2) ;
	shutter    = option.GetDouble("-shutter",  SHUTTER,    3) ;
	s_motion   = option.GetInt   ("-smotion",  S_MOTION,   3) ;
	s_shutter  = option.GetDouble("-sshutter", S_SHUTTER,  3) ;
	lid        = option.Get      ("-NOLID",                3) ;
	bg_color   = option.GetInt   ("-bgcolor",  BG_COLOR,   3) ;

	is_set_fog = fogFlag = option.GetInt("-fog", -1, 4) ;
	if (is_set_fog < 0 || is_set_fog > 3)
	{
		// default
		if (envOpenGL != SAVAGE2000)
			fogFlag = FOG_MODE ;
		else
			fogFlag = FOG_MODE_SAVAGE2000 ;

		is_set_fog = FALSE ;
	}
	else
		is_set_fog = TRUE ;

	dither     = option.GetInt   ("-dither",   DITHER,     3) ;
	light      = option.GetInt   ("-light",    LIGHT,      2) ;
	if      (light < 0) light = 0 ;
	else if (light > 8) light = 8 ;

	local      = option.Get      ("-LOCAL",                3) ;
	lightType  = option.GetInt   ("-ltype",    LIGHT_TYPE, 3) ;

	spinSpeed  = option.GetDouble("-speed",    SPIN_SPEED, 6) ;

	// ��ž�饤�ȥե饰��-1����ư��0��OFF��1��ON��
	rotateLight = option.GetInt("-lrotate", ROTATE_LIGHT, 3) ;


	// �������졼�����Хåե�
	if (envOpenGL == SAVAGE2000)
		donotUse_accum_buffer = TRUE ;
	else
		donotUse_accum_buffer = option.Get("-NOACCUM", 5) ;

	if (donotUse_accum_buffer || !option.Get("-ACCUM", 3))
		glut_accum = 0x0000 ;
	else
		glut_accum = GLUT_ACCUM ;


	// Color Buffer RGBA �⡼��
	if (option.Get("-RGBA", 5))
		glut_alpha = GLUT_ALPHA ;
	else
		glut_alpha = 0x0000 ;
}


void InitVarGlclockMarkAndLight(Option& option)
{
	glclock_mark = option.GetDouble("-mark", GLCLOCK_MARK, 5) ;
	InitGlClockMark(glclock_mark) ;

	if (glclock_mark != (float)NO_GLCLOCK_MARK_LIGHT_TEST &&
		glclock_mark != (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST &&
		glclock_mark != (float)NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST)
		light_vividness = option.GetDouble("-lsaturation", LIGHT_VIVIDNESS, 3) ;

	pre_fps_message = option.GetString("-prefpsmessage", PRE_FPS_MESSAGE, 4) ;

	solid_saturation = option.GetDouble("-ssaturation", SOLID_SATURATION, 4) ;
	solid_contrast = option.GetDouble("-scontrast", SOLID_CONTRAST, 3) ;

	// �饤�Ȥκ��٤��������ʤ��� lcol[][] �˥��å�
	SetLightColorTableFromSaturation() ;


	spec = option.GetDouble("-specular", SPECULAR, 3) ;

	// -r �Υ��ڥ���顼�ƥ���������طʥ����
	specularMapReflectGamma = option.GetDouble("-rgamma", R_SPECULAR_MAP_GAMMA, 7) ;

	// -DETAIL �Υ��������
	detailMapGamma = option.GetDouble("-dgamma", IS_SET_DETAIL_MAP_GAMMA, 7) ;

	masterBright = option.GetDouble("-bright",   MASTER_BRIGHT, 3) ;
}


void InitVarTexture(Option& option)
{
	image_path = option.GetString("-ipath",    ENV_IMAGE_PATH, 3) ;
	{
		char *epath = getenv(ENV_IMAGE_PATH) ;
		if (epath)
			env_path = epath ;
		else
			env_path = "" ;
	}

	// �ߥåץޥåץƥ���
	mipmapTest = option.Get("-MTEST", 3) ;

	// �ǥե���ȥƥ������㥵����
	if (mipmapTest)
		defaultTextureSize = option.GetInt("-texsize", DEFAULT_TEXTURE_SIZE_MIPMAPTEST, 8) ;
	else
		defaultTextureSize = option.GetInt("-texsize", DEFAULT_TEXTURE_SIZE, 8) ;

	if (defaultTextureSize < 0)
		defaultTextureSize = 16 ;

	defaultTextureSize = Pow(2, defaultTextureSize) ;


	if (glclock_mark != (float)NO_GLCLOCK_MARK_FILTER_TEST)
		filterTest = option.Get("-FTEST", 3) ;

	if (mipmapTest)
		filterTest = TRUE ;

	if (glclock_mark != (float)NO_GLCLOCK_MARK_FOG_TEST &&
		glclock_mark != (float)NO_GLCLOCK_MARK_FOG_TEX_TEST)
		maxTriTest = option.Get("-MAXTRITEST", 4) ;

	// ����åɥƥ��������-s -r ���ץ������ˤ򸵤Υ��᡼���̤�˹Ԥ�
	// �ܡ��ɥ��顼���򡢥ƥ������㥹������� 20.0f����ž�� 20.0f ��ǥե���Ȥ˥��å�
	orgImageFlag = option.Get("-ORGIMAGE", 4) ;

	if (filterTest)
	{
		solid_file = option.GetString("-sfile",    FILTER_TEST_FILE, 3) ;

		if (mipmapTest)
		{
			s_scale    = option.GetDouble("-sscale",   MIPMAP_TEST_TEX_SCALE,   3) ;
			t_scale    = option.GetDouble("-tscale",   MIPMAP_TEST_TEX_SCALE,   3) ;
		}
		else
		{
			s_scale    = option.GetDouble("-sscale",   FILTER_TEST_TEX_SCALE,   3) ;
			t_scale    = option.GetDouble("-tscale",   FILTER_TEST_TEX_SCALE,   3) ;
		}
	}
	else
	{
		solid_file = option.GetString("-sfile",    SOLID_FILE,  3) ;

		if (orgImageFlag)
		{
			s_scale    = option.GetDouble("-sscale",   ORG_IMAGE_TEX_SCALE,   3) ;
			t_scale    = option.GetDouble("-tscale",   ORG_IMAGE_TEX_SCALE,   3) ;
		}
		else
		{
			s_scale    = option.GetDouble("-sscale",   TEX_SCALE,   3) ;
			t_scale    = option.GetDouble("-tscale",   TEX_SCALE,   3) ;
		}
	}

	// ����åɥƥ��������ž
	if (orgImageFlag)
		tex_rotate = option.GetDouble("-trotate", ORG_IMAGE_TEX_ROTATE, 5) ;
	else
		tex_rotate = option.GetDouble("-trotate", TEX_ROTATE, 5) ;


	sld2_file  = option.GetString("-sfile",    SLD2_FILE,   3) ;
	if (solid_file == sld2_file)
		sld2_file = "" ;

	env_file   = option.GetString("-efile",    ENV_FILE,    3) ;
	light_file = option.GetString("-lfile",    LIGHT_FILE,  3) ;
//	detail_file = option.GetString("-dfile",   DETAIL_FILE, 3) ;
	detail_file = option.GetString("-dfile",   IS_SET_DETAIL_FILE, 3) ;

	float tex_scale ;
	tex_scale  = option.GetDouble("-texscale",  0.0f,        2) ;
	dtex_scale = option.GetDouble("-dtexscale", IS_SET_DETAIL_TEX_SCALE, 3) ;

//	max_tex    = option.Get      ("-maxtex",                3) ;
	tex_size   = option.GetInt   ("-isize",    TEX_SIZE,    2) ;

	env        = option.GetInt   ("-env",      ENV_TEX,    2) ;
	if (env < 0) env = 0 ;
	else if (env > 2) env = 2 ;
	texture = env ;

	int    sol ;
	if (glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_TEST ||
		glclock_mark == (float)NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST ||
		glclock_mark == (float)NO_GLCLOCK_MARK_FILTER_TEST ||
		glclock_mark == (float)NO_GLCLOCK_MARK_FOG_TEX_TEST)
		sol = solid ;	// -mark 2.0, 2.1, 3.0, 7.1 ���ϼ�ưŪ�˥��åȤ����
	else
	{
		sol = option.GetInt   ("-solid",    SOLID_TEX,  2) ;
	}

	if (sol)
	{
		texture = solid = sol ;
		env = 0 ;
	}

	textureFilter = option.GetString("-filter",   MIN_FILTER, 3) ;

	if (textureFilter == String(MIN_FILTER))
	{
		int filter ;

		filter = option.Get("-N") ;
		if (filter) textureFilter = String("N") ;

		filter = option.Get("-L") ;
		if (filter) textureFilter = String("L") ;

		filter = option.Get("-NN") ;
		if (filter) textureFilter = String("NN") ;

		filter = option.Get("-LN") ;
		if (filter) textureFilter = String("LN") ;

		filter = option.Get("-NL") ;
		if (filter) textureFilter = String("NL") ;

		filter = option.Get("-LL") ;
		if (filter) textureFilter = String("LL") ;
	}

	// ���󥢥����ȥ��ԥå��ե��륿��max of max anisotropy��
	default_maxAnisotropy = option.GetDouble("-anisotropy", NONE, 4) ;
	if (default_maxAnisotropy >= 0.0f && default_maxAnisotropy < 1.0f)
		default_maxAnisotropy = 1.0f ;


	// ����åɥƥ������㥹������
	if (tex_scale != 0.0f) { s_scale = t_scale = tex_scale ; }
	if (s_scale == 0.0f) s_scale = TEX_SCALE ;
	if (t_scale == 0.0f) t_scale = TEX_SCALE ;

	if (tex_size > 30) tex_size = 30 ;

	cacheImages = option.Get("-CACHEIMAGE", 6) ;
//	donotUsecacheImages = TRUE ;

	conserveTex = option.Get("-CONSTEX", 8) ;
}


void InitVarOpenGLLimitation(Option& option)
{
	// OpenGL �С����������
	use_OpenGL_1_0 = option.Get("-OpenGL-1.0", 11) ;
	use_OpenGL_1_1 = option.Get("-OpenGL-1.1", 11) ;

	if (use_OpenGL_1_0)
	{
		donotuse_OpenGL_1_1 = TRUE ;
		donotuse_OpenGL_1_2 = TRUE ;
	}
	else if (use_OpenGL_1_1)
	{
		donotuse_OpenGL_1_2 = TRUE ;
	}

	// OpenGL ��ĥ��ǽ����ڶػ�
	donotUse_extensions = option.Get("-NO_GL_EXTENSIONS", 10) ;

	donotUse_ARB_multitexture  = option.Get("-NO_ARB_multitexture",  16) || donotUse_extensions ;
	donotUse_EXT_multitexture  = option.Get("-NO_EXT_multitexture",  16) || donotUse_extensions ;
	donotUse_SGIS_multitexture = option.Get("-NO_SGIS_multitexture", 17) || donotUse_extensions ;
	donotUse_multitexture      = option.Get("-NO_multitexture",      12) || donotuse_OpenGL_1_1 ;

	limit_maxTextureUnits      = option.GetInt("-maxtexunits", LIMIT_MAX_TEXTURE_UNITS, 11) ;
	if (limit_maxTextureUnits == 1)
		donotUse_multitexture = TRUE ;

	// 3Dfx OpenGL ICD, 3Dfx OpenGL Beta, 3Dfx Mesa, G400 �⡼�ɤǤϡ��ޥ���ƥ��������Ȥ�ʤ�
	if (envOpenGL == VOODOO_3DFX_ICD || fxOpenGL || envOpenGL == VOODOO_MESA_OLD || envOpenGL == VOODOO_MESA || envOpenGL == G400)
		donotUse_multitexture = TRUE ;

	donotUse_ARB_separate_specular_color = option.Get("-NO_ARB_separate_specular_color",  16) || donotUse_extensions ;
	donotUse_EXT_separate_specular_color = option.Get("-NO_EXT_separate_specular_color",  16) || donotUse_extensions ;
	donotUse_separate_specular_color     = option.Get("-NO_separate_specular_color",      12) || donotuse_OpenGL_1_2 ;

	if (envOpenGL == VOODOO_MESA || envOpenGL == VOODOO_MESA_OLD)
		donotUse_separate_specular_color = TRUE ;	// 3Dfx Mesa �⡼�ɤǤϡ�Separate Specular Color ��Ȥ�ʤ�

	donotUse_EXT_texture_filter_anisotropic = option.Get("-NO_EXT_texture_filter_anisotropic",  26) || donotUse_extensions ;
	donotUse_texture_filter_anisotropic     = option.Get("-NO_texture_filter_anisotropic",      22) ;

	donotUse_EXT_texture_object = option.Get("-NO_EXT_texture_object",  19) || donotUse_extensions ;
	donotUse_texture_object     = option.Get("-NO_texture_object",      15) || donotuse_OpenGL_1_1 ;

	donotUse_EXT_polygon_offset = option.Get("-NO_EXT_polygon_offset",  19) || donotUse_extensions ;
	donotUse_polygon_offset     = option.Get("-NO_polygon_offset",      15) || donotuse_OpenGL_1_1 ;

	donotUse_ARB_texture_cube_map = option.Get("-NO_ARB_texture_cube_map",  20) || donotUse_extensions ;
	donotUse_EXT_texture_cube_map = option.Get("-NO_EXT_texture_cube_map",  20) || donotUse_extensions ;
	donotUse_NV_texgen_reflection = option.Get("-NO_NV_texgen_reflection",  17) || donotUse_extensions ;
	donotUse_texgen_reflection    = option.Get("-NO_texgen_reflection",     14) ;
	fakeSphereMap                 = option.Get("-FAKESPHEREMAP",             5) ;

	donotUse_ARB_texture_compression = option.Get("-NO_ARB_texture_compression", 20) || donotUse_extensions ;
	donotUse_texture_compression     = option.Get("-NO_texture_compression",     16) || donotuse_OpenGL_1_1 ;

	donotUse_EXT_texture_compression_s3tc = option.Get("-NO_EXT_texture_compression_s3tc", 32) || donotUse_extensions ;
	donotUse_texture_compression_s3tc     = option.Get("-NO_texture_compression_s3tc",     28) || donotuse_OpenGL_1_1 ;

	donotUse_3DFX_texture_compression_FXT1 = option.Get("-NO_3DFX_texture_compression_FXT1", 33) || donotUse_extensions ;
	donotUse_texture_compression_FXT1      = option.Get("-NO_texture_compression_FXT1",     28) || donotuse_OpenGL_1_1 ;


	// Riva128, PERMEDIA2 �Ǥϡ�GL_ONE, GL_ONE_MINUS_SRC_COLOR �֥��ɤ�Ȥ��ʤ�
	if (envOpenGL == PERMEDIA2 || envOpenGL == RIVA) donotUseBlend_AddSmooth = TRUE ;
	else donotUseBlend_AddSmooth = option.Get("-NO_ADDSMOOTH_BLEND", 13) ;

	if (envOpenGL == PERMEDIA2) donotUseBlend_Multiply = TRUE ;
	else donotUseBlend_Multiply = option.Get("-NO_MULTIPLY_BLEND", 9) ;


	detailBlend = option.GetInt("-dblend", DETAIL_BLEND, 3) ;
	specularBlend = option.GetInt("-sblend", SPECULAR_BLEND, 3) ;
}


void InitVarRenderingMode(Option& option)
{
	crystal    = option.Get      ("-CRYSTAL",               2) ;

//	crystal_a  = option.GetDouble("-calpha",   CRYSTAL_A,   3) ;
	reality    = option.GetInt   ("-real",     REALITY,     2) ;

	reflection = option.GetInt   ("-reflect",  REFLECTION,  4) ;
	if (reflection < 0)	// == REFLECTION
		reflection = option.Get      ("-REFLECT",               2) ;

	detail     = option.GetInt   ("-detailtex", DETAIL_TEX, 3) ;
	if (detail < 0)	// == DETAIL_TEX)
		detail     = option.Get      ("-DETAILTEX",             2) ;

	if (!reality)
	{
		reflection = FALSE ;

		// 2000.06.15
		if (env < 1)
			detail = FALSE ;
	}
	else
	{
		solid = 0 ;
		env = 0 ;
		crystal = 0 ;
	}

	// -FTEST -MTEST �ǥǥե���Ȥξ��ϡ������� -s 1 ���å�
	if ((mipmapTest || filterTest) && !reality && !solid && !env && !crystal)
	{
		solid = 1 ;
		texture = 1 ;
	}


	u_smooth   = option.GetInt   ("-usmooth",  U_SMOOTH,    2) ;
	v_smooth   = option.GetInt   ("-vsmooth",  V_SMOOTH,    2) ;

	if (glclock_mark != (float)NO_GLCLOCK_MARK_LIGHT_TEST)
		g_smooth = option.GetInt("-gsmooth", G_SMOOTH, 3) ;

	// ���֥� or ���󥰥�Хåե�
	glut_buff  = option.Get("-SINGLE", 2) ;
	if (glut_buff) glut_buff = GLUT_SINGLE ;
	else           glut_buff = GLUT_DOUBLE ;


	lightTwoSide = option.Get("-LIGHTTWOSIDE", 6) ;
	if (envOpenGL == VOODOO)
		lightTwoSide = TRUE ;	// 3Dfx OpenGL Beta 2.1 �Ǥϡ����Τ� GL_LIGHT_MODEL_TWO_SIDE ��������®

	noCullObj = option.Get("-NOCULLOBJ", 7) ;
	cullObj   = option.Get("-CULLOBJ",   5) ;	// ɽ�̥��֥������Ȥ򤹤٤ƥ��å�


	n_width    = option.GetDouble("-nwidth",   N_WIDTH,     2) ;
	if      (n_width < 0.0f) n_width = 0.0f ;
	else if (n_width > 2.0f) n_width = 2.0f ;


	float zOff = option.GetDouble("-zoffset",  zOffset,     2) ;
	if (zOff != zOffset)
	{
		isSet_zOffset = TRUE ;
		zOffset = zOff ;
	}

	zOffset    = option.GetDouble("-zoffset",  zOffset,     2) ;
//#ifndef GL_VERSION_1_1
//	zOffset = 0.0f ;
//#endif	// #ifndef GL_VERSION_1_1
//	if (zOffset > 0.0f) polygon_offset = TRUE ;


	// �ɤ�Ĥ֤��⡼��
	String pol_mode ;
	pol_mode   = option.GetString("-pmode",    POL_MODE,    3) ;

	// �ѡ����ڥ��ƥ��֥��쥯�����ҥ��
	String pers_corr ;
	pers_corr  = option.GetString("-pers",     PERS_CORR,   2) ;

	// �ե����ҥ��
	String fog_hint ;
	fog_hint   = option.GetString("-fhint",    FOG_HINT,   2) ;

	if      (String("P")  == pol_mode) gl_pol_mode = GL_POINT ;
	else if (String("L")  == pol_mode) gl_pol_mode = GL_LINE ;
	else                               gl_pol_mode = GL_FILL ;

	gl_pers_corr = GetHintMode(pers_corr) ;
	gl_fog_hint  = GetHintMode(fog_hint) ;
}


void InitVarBenchmarkMode(Option& option)
{
	// �����ޡ����ٻ���ʥ٥���ޡ����ѡ�
	timerResolution = (unsigned int)option.GetInt("-tresolution", TIME_RESOLUTION, 5) ;

	if (glclock_mark == 0.0f)
		benchmark = option.Get("-BENCHMARK", 2) ;


	// Texture Internal Format
	String tformat = option.GetString("-tformat", "", 3) ;
	if (tformat != "")
	{
		isSet_texFormat = TRUE ;
		tex_format = tformat ;
	}
	else
	{
		isSet_texFormat = FALSE ;

		if (!benchmark)
			tex_format = TEX_FORMAT ;
		else
			tex_format = TEX_FORMAT_BENCHMARK ;
	}

	if (option.Get("-TC", 3))
	{
		isSet_texFormat = TRUE ;
		tex_format = TEX_FORMAT_COMPRESSED ;
	}
	else if (option.Get("-S3TC", 5))
	{
		isSet_texFormat = TRUE ;
		tex_format = TEX_FORMAT_COMPRESSED_S3TC ;
	}
	else if (option.Get("-FXT1", 5))
	{
		isSet_texFormat = TRUE ;
		tex_format = TEX_FORMAT_COMPRESSED_FXT1 ;
	}


//	if (!saver && glclock_mark == 0.0f)
	if (!saver)
		title  = option.GetString("-title", TITLE_VER,  3) ;

	fps_count = -1 ;
	master_fps_count = -1 ;

	if (benchmark)
	{
		// �٥���ޡ����κǽ�ΰ��ϡ�-wfps 5000 ���٤Υ������Ȥ򤪤��٤�
		noCullObj = TRUE ;
		wait_calc_fps = option.GetInt("-wfps", FPS_CALC_WAIT_BENCH, 3) ;
	}
	else
	{
		wait_calc_fps = option.GetInt("-wfps", FPS_CALC_WAIT, 3) ;
	}


	stop = option.Get("-STOP", 3) ;

	// -STOP ���ץ������ϥ���󥰤ʤ�
	if (stop)
		noCullObj = TRUE ;

	m_sleep    = option.GetInt   ("-sleep",    M_SLEEP,     3) ;
	if (m_sleep <= 0) us_sleep = 0 ;
	else              us_sleep = (unsigned int)m_sleep * 1000 ;


//	no_sign    = option.Get      ("-NOSIGN",                4) ;
	sign       = option.GetString("-sign",     SIGN,        5) ;
	// �٥���ޡ��� �ޤ��ϡ�����򥻥åȤ��줿���ϥ���������ʤ�
	if (benchmark) sign = "" ;
	if (sign == "")
		no_sign = TRUE ;

//	fout_fps   = option.GetString("-foutfps",               3) ;
	fout_fps   = option.GetString("-foutfps",  FOUT_FPS,    3) ;
	fout_gl    = option.GetString("-foutgl",   FOUT_GL,     7) ;

//	sleepRatio = option.GetDouble("-sratio",   SLEEP_RATIO, 3) ;
/*
	if (sleepRatio > 0.0f)
	{
		// ����Ǥ⣱����
		if      (sleepRatio < 0.1f) sleepRatio = 0.1f ;
		// ��������Ķ��ξ��ϻ���ʤ��ȸ��ʤ�
		else if (sleepRatio > 1.0f) sleepRatio = 0.0f ;
	}
*/

	// ��°�ƥ�������ΰ����򣱣�������������
	if (glclock_mark != 0)
		conserveTex = TRUE ;
}


void InitVarClockQuality(Option& option)
{
	if ((clock_quality = option.GetInt("-quality", QUALITY, 2)) < 1) clock_quality = 1 ;
	else if (                                     clock_quality > 8) clock_quality = 8 ;

	SetQuality(clock_quality) ;

	// explicit_clock_div != NONE �ξ������Ū�˻��ꤵ��Ƥ���
	explicit_clock_div = option.GetInt("-divide", NONE,    2) ;
	if (explicit_clock_div != NONE)
		clock_div = explicit_clock_div ;

	clock_hedron = option.GetInt("-hedron", clock_hedron, 4) ;
	clock_glass  = option.GetInt("-glass",  clock_glass,  2) ;
	clock_point  = option.GetInt("-bit",    clock_point,  3) ;
	clock_mesh   = option.GetInt("-mesh",   clock_mesh,   3) ;
	gold_depth   = option.GetInt("-gold",   gold_depth,   3) ;

	if (clock_div < 3) clock_div = 3 ;
	if (clock_div < 6 && gold_depth >= 1) gold_depth = 0 ;

	if      (clock_glass < 0) clock_glass = 0 ;
	else if (clock_glass > 3) clock_glass = 3 ;
}


void AdjustSuperSampling()
{
	// anti ��Ŭ�����ͤ�
	switch(anti)
	{
	case 7: anti = 8 ; break ;
	case 10:
	case 11: anti = 12 ; break ;
	case 13:
	case 14:
	case 15: anti = 16 ; break ;
	default:
		if (anti <= 1) anti = 0 ;
		else if (anti > 16) anti = 16 ;
		break ;
	}

	// field ��Ŭ�����ͤˡ�2, 3, 4, 5, 7, 12 �Τ����줫�ˡ�
	switch(field)
	{
	case 6:
	case 7: field = 7 ; break ;
	case 10:
	case 11:
	case 12: field = 12 ; break ;
	default:
		if (field <= 1) field = 0 ;
		else if (field > 12) field = 12 ;
		break ;
	}

	if      (motion   < 2                    ) motion   = 0 ;
	else if (motion   > MAX_MOTION_N_SAMPLING) motion   = MAX_MOTION_N_SAMPLING ;
	if      (s_motion < 2                    ) s_motion = 0 ;

	// ���ץ�����Ʊ���˻��ꤵ�줿����Ʊ�����������
	if (motion && anti)
		multi_super_sample = TRUE ;


	if (anti)   default_anti   = anti ;
	else        default_anti   = N_ANTI ;

	if (field)  default_field  = field ;
	else        default_field  = N_FIELD ;

	if (motion) default_motion = motion ;
	else        default_motion = N_MOTION ;
	pre_motion = 0 ;

	if (s_motion) pre_s_motion = s_motion ;
	else          pre_s_motion = N_MOTION ;


	if (anti || motion)
		field = 0 ;

	// shutter ������
	if (shutter != 1.0f)
	{
		// �����⡼�ɰʳ��ϡ���餫�����
		if (shutter > 1.0f - (1.0f / default_motion))
			shutter = 1.0f - (1.0f / default_motion) ;
	}
}


void InitVarMisc(Option& option)
{
	// �����
	t_ratio		= T_RATIO ;
	i_ratio		= I_RATIO ;
	t_max		= T_MAX ;
	p_min		= P_MIN ;
	p_max		= P_MAX ;
	ir_ratio	= IR_RATIO ;

	AdjustSpinAndFrameCount(spin_x, dst_fps_count, 0.0) ;

	spin_y = SPIN_Y ;
	dist_y = B_DIST ;

	last_x = mouse_x = xsize / 2 ;
	last_y = mouse_y = ysize / 2 ;

	if (lid) t_roof = 0.0f ;
	else     t_roof = 1.0f ;

	p_step = yp_step = z_step = 0.0f ;
	DragL = DragR = 0 ;

	now.h_rotate = 0.0f ;
	now.g_spin = 0.0f ;

	// �ѡ��������
//	now.pers = pre.pers = 1.0f ;
	float zoom = option.GetDouble("-zoom", ZOOM, 4) ;
	if      (zoom < 0.0f) zoom = 0.0f ;
	else if (zoom > 1.2f) zoom = 1.2f ;
	// 0.0f��1.0f = 2.0f^3��2.0f^-3��8��1��1/8��
	now.pers = now.y_pers = pre.pers = pre.y_pers = pow(2.0f, (0.5f - (zoom)) * 6.0f) ;

	now.clock_z = pre.clock_z = B_DIST ;
	now.roof = pre.roof = 0.0f ;

	// FPS ��Ϣ
	f_cnt = 1 ;
	f_time = 1 ;
	f_cal_flag = FALSE ;

	nm_polygons = 0 ;


	// �Хå����饦��ɥ��顼
	if (bg_color > 4) bg_color = 0 ;

	if (saver || (crystal && !bg_color))
	{
		// �����꡼�󥻡��Х⡼�ɤ⤷���ϡ����ꥹ����� bg_color == 0�ʥǥե���ȡˤξ�硢�طʿ��Ϲ��Τޤ��Ѳ��ʤ�
		currentBG[0] = 0.0f ;
		currentBG[1] = 0.0f ;
		currentBG[2] = 0.0f ;
		currentBG[3] = 1.0f ;
	}
	else
	{
		currentBG[0] = bgColor[bg_color].v[0] ;
		currentBG[1] = bgColor[bg_color].v[1] ;
		currentBG[2] = bgColor[bg_color].v[2] ;
		currentBG[3] = 1.0f ;
	}

	// LIGHT0 ���顼
	currentCL[0] = currentLC[0] = lightCl[0].v[0] ;
	currentCL[1] = currentLC[1] = lightCl[0].v[1] ;
	currentCL[2] = currentLC[2] = lightCl[0].v[2] ;
	currentCL[3] = currentLC[3] = 1.0f ;

	// �����׽�����Mesa��
	if (mesa)
	{
		if (currentCL[0] > 1.0f) currentCL[0] = 1.0f ;
		if (currentCL[1] > 1.0f) currentCL[1] = 1.0f ;
		if (currentCL[2] > 1.0f) currentCL[2] = 1.0f ;
	}

	currentGC[0] = currentCL[0] ;
	currentGC[1] = currentCL[1] ;
	currentGC[2] = currentCL[2] ;

	// �ƥ�������֥����ѤΥ����ȥ��顼�ʥ��ڥ���顼�ѡ�
	currentSP[0] = 1.0 + (currentCL[0] - 1.0) * specularTimeGradRatio ;
	currentSP[1] = 1.0 + (currentCL[1] - 1.0) * specularTimeGradRatio ;
	currentSP[2] = 1.0 + (currentCL[2] - 1.0) * specularTimeGradRatio ;
	currentSP[3] = 1.0f ;

	timeData.t = time(NULL) ;
	timeData.t_area = localtime(&timeData.t) ;

	timeData.p_hour   = timeData.n_hour   = timeData.t_area->tm_hour % 12 ;
	timeData.p_minute = timeData.n_minute = timeData.t_area->tm_min ;
	timeData.p_second = timeData.n_second = timeData.t_area->tm_sec ;

//	FramesPerSecond(FALSE) ;
	FramesPerSecond() ;
	f_cal_flag = TRUE ;

	if (stop || benchmark) now.roof = pre.roof = 1.0f ;

#ifdef WIN32
	g_hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "GLCLOCK") ;
	if (g_hFileMap)
		g_pFps = (char*)MapViewOfFile(g_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0) ;
#endif

#ifdef GIFF_DEMO
	spin_x = GIFF_SPIN_X ;
#endif
}


void InitVarAdjust()
{
	// ɽ�̥��饹��ˡ���Τ��餷�졼��
	if (local) norm_ratio = 0.5f ;
	else       norm_ratio = 1.0f ;

	n_golden = clock_div ;
	if (n_golden > 8) n_golden = 8 ;
	if (clock_div == 3) n_golden = 0 ;

	if (glclock_mark != (float)NO_GLCLOCK_MARK_FILTER_TEST)
		SetTextureFilter() ;


	if (reality < 0) reality = 0 ;
	if (reality)
	{
//		if (reality > 3) reality = 3 ;
		if (reality > 2) reality = 2 ;
		solid = 1 ;
		texture = 3 ;
	}

	if (solid) solid = 1 ;
	if      (texture <= 0) { texture = 0 ; solid = 0 ; }	// �ƥ�������ʤ��ʤ� solid = 0 ��
	else if (texture >  3) texture = 3 ;

	if (crystal < 0) crystal = 0 ;
//	if (crystal > 2) crystal = 2 ;
	if (crystal) texture = 3 ;	// ���ꥹ����ʤ� texture = 3 ��

//	// �ƥ�������ޥåԥ󥰻��ȡ����פ����ѷ��ʲ��λ��� glass != 1 ��
//	if (clock_glass == 1 && (texture || clock_div <= 6)) clock_glass = 2 ;


	radius_ratio = RadiusRatio(clock_div) ;


	// �Ķ��ޥåԥ󥰤���Ѥ�����ϡ���å���κ�ɸ�ͤ�¿���ѹ�
	if ((texture && !solid) || reality)
	{
		s_v[4][0] = 16.0f ; s_v[4][1] = 3.7f ;	// -e -r �λ� s_v[4] = (16.3f, 3.4f) ��
		s_v[8][0] = 18.6f ; s_v[8][1] = 1.3f ;	// -e -r �λ� s_v[8] = (19.0f, 1.1f) ��
	}
}


// ���ץ����������ѿ������
void InitVar(Option& option)
{
	// ���ޥ�ɥ饤�󥪥ץ����

#ifdef WIN32
	// Win32 ��
	// �������Ǥ� Mesa �γ�ĥ���ޥ����
	//
	// "opengl32.dll" ������Ū�˥�󥯤�����ϡ�
	// ɬ��¾�� OpenGL ���ޥ�ɤ�Ʊ�� DLL ������ɤ���
	// ɬ�� Extension ��Ϣ�Υ��ݡ��ȥ����å�������˥����ɤ��뤳��
	//
	// "opengl32.dll" �����Ū�˥�󥯤�������ä�ɬ�פʤ�
	// Extension ��Ϣ�Υ��ݡ��ȥ����å����˼�ưŪ�� "opengl32.dll" ��������ɤ����
	openGL32_DLL = OPENGL32_DLL ;
	OpenGLDLLCtrl::openGLDLLCtrl.LoadLibrary(openGL32_DLL) ;

	iconNo = option.GetInt("-icon", ICON_NO, 3) ;
	if (iconNo < 0 || iconNo > 3)
		iconNo = 0 ;

#endif	// #ifdef WIN32

	// glut Game Mode
	// When this option is set, -w / -h / -FU / -xp / -yp options will be all ignored.
	glut_gameModeString = option.GetString("-fullscreen", DEFAULT_GAME_MODE_STRING, 3) ;
	if (glut_gameModeString != DEFAULT_GAME_MODE_STRING)
		glut_gameMode = TRUE ;

#ifndef IS_SUPPORTED_GLUT_GAME_MODE
	glut_gameMode = NULL ;
#endif	// #ifndef IS_SUPPORTED_GLUT_GAME_MODE


	// OpenGL�Ķ��ʥɥ饤�С��ӥǥ������ɼ��̡�
	InitVarEnvOpenGL(option) ;

	// Window �⡼�ɴ�Ϣ
	InitVarWindow(option) ;

	// ���ģãǥ�����󥰴�Ϣ
	InitVarRendering(option) ;

	// glclock Mark
	InitVarGlclockMarkAndLight(option) ;

	// Texture ��Ϣ
	InitVarTexture(option) ;

	// OpenGL ����
	InitVarOpenGLLimitation(option) ;

	// ������󥰥⡼����
	InitVarRenderingMode(option) ;

	// �٥���ޡ����⡼�ɤˤ�äƱƶ��������������
	InitVarBenchmarkMode(option) ;

	// ���ץ�����ƥ�
	InitVarClockQuality(option) ;

	// �����ѥ���ץ�󥰲��������
	AdjustSuperSampling() ;

	// ����¾�ͤ�������
	InitVarAdjust() ;

	// ����¾�����
	InitVarMisc(option) ;
}


// glut �����
void InitWindow(Option& option)
{
	int argc = option.ArgC(), x = 0, y = 0 ;

	// -foutgl ���ץ���󤬻��ꤵ�줿���
	// �С������������Ϥ�������ǽ�λ

	// Window Setup
#if defined WIN32 // && defined VOODOO_MODE
	if (fout_gl == String("") && !glut_gameMode)
	{
		isChange_DisplaySettings = SetDisplayMode(xsize, ysize, cw, ch) ;
		SetDisplayBPP(bpp, cw, ch) ;
	}
#endif

#ifdef macintosh
	char **argv = option.ArgV() ;
	glutInit(&argc, argv) ;
#else
	glutInit(&argc, option.ArgV()) ;
#endif

	glutInitDisplayMode(glut_buff | GLUT_RGB | GLUT_DEPTH | glut_accum | glut_alpha) ; 

	if ((x = option.GetInt("-xposition", NONE, 3)) != NONE &&
	    (y = option.GetInt("-yposition", NONE, 3)) != NONE && !glut_gameMode)
		glutInitWindowPosition(x, y) ;

#ifdef IS_SUPPORTED_GLUT_GAME_MODE
	if (glut_gameMode)
	{
		glutGameModeString(glut_gameModeString) ;
		winIDglut = glutEnterGameMode() ;
	}
#endif

	if (!glut_gameMode)
	{
		glutInitWindowSize(xsize, ysize) ;
		winIDglut = glutCreateWindow(title) ;
	}


#ifdef IS_SUPPORTED_GLUT_SET_CURSOR
	if (saver) glutSetCursor(GLUT_CURSOR_NONE) ;
#endif	// #ifdef IS_SUPPORTED_GLUT_SET_CURSOR


#ifdef IS_SUPPORTED_GLUT_KEYBOARD_STATUS
	glutIgnoreKeyRepeat(TRUE) ;
	isSupported_glutKeyboardStatus = TRUE ;

#else	// #ifdef IS_SUPPORTED_GLUT_KEYBOARD_STATUS
	isSupported_glutKeyboardStatus = FALSE ;
#endif	// #ifdef IS_SUPPORTED_GLUT_KEYBOARD_STATUS ... #else


//	glutCreateSubWindow(winID, 0,0, 160,160) ;
//	glutSetWindow(winID) ;

/*
	if (full)
	{
		glutFullScreen() ;
		logprintf("Enable full screen.\n") ;
	}
*/
/*
#ifdef WIN32
	if (saver)
	{
		HWND win ;
		win = FindWindow("GLUT", title) ;
		if (win)
		{
			SetWindowPos(win, HWND_NOTOPMOST,
						 0, 0,
						 GetSystemMetrics(SM_CXSCREEN),
						 GetSystemMetrics(SM_CYSCREEN),
						 SWP_NOREDRAW | SWP_NOMOVE) ;
		}
	}
#endif
*/
}

// �ǥץ��Хåե����å�
void SetDepthBuffer()
{
	glClearDepth(1.0f) ;
	glEnable(GL_DEPTH_TEST) ;
	glDepthFunc(GL_LESS) ;
}


// OpenGL �Υ�󥿥��� Renderer �ˤ�äƶػߵ�ǽ�ʤɤ򥻥å�
void AdjustRunTimeOpenGLEnvironmentFromRenderer()
{
/*
#if defined WIN32
	int org_envOpenGL = envOpenGL ;
#endif
*/
	String vendor	= Upper(glVendor) ;
	String renderer	= Upper(glRenderer) ;
	String version	= Upper(glVersion) ;


	// ������ǧ�� OFF ����-NO_AUTO�ˤ� ����Ū�˴Ķ������ꤵ��Ƥ�����
	if (donotUse_AutoSelectOpenGLEnvironmentMode ||
		(envOpenGL != GENERIC && envOpenGL != VOODOO_AUTO))
		return ;

	// OpenGL Renderer �ˤ�äơ��ػߵ�ǽ�ʤɤ򥻥å�

	// �٥���ޡ����ʳ��Υ⡼�ɤǤΤߥ��å�
	// ®�٤˰��ƶ��νФ��Τʤ�
	if (!benchmark)
	{
		// Riva TNT ���꡼��
		if (renderer.Search("TNT"))
		{
			envOpenGL = RIVA_TNT_FIX ;

			// ���׳ѿ�������Ū�˻��ꤵ��Ƥ��ʤ�����
			// �����ѷ����������
			if (explicit_clock_div == NONE && clock_div > 58)
				clock_div = 58 ;

			if (!isSet_zOffset)
				zOffset = Z_OFFSET_TNT_FIX ;
		}

/*
		// 3Dfx ICD or Q3 stand-alone
		else if (vendor.Search("3DFX") &&
				 (version.Search("STANT-ALONE") ||
				  renderer.Search("STANT-ALONE") ||
				  renderer.Search("ICD")))
		{
			envOpenGL = VOODOO_3DFX_ICD ;

			if (!isSet_zOffset)
				zOffset = Z_OFFSET_TNT_FIX ;

			donotUse_multitexture = TRUE ;

			// Voodoo �⡼�ɻ����Ĥޤ� 3Dfx stand-alone �ʤ�
			if (version.Search("STANT-ALONE") ||
				renderer.Search("STANT-ALONE"))
				voodoo = TRUE ;
		}

		// 3Dfx OpenGL Beta 2.1
		else if (version.Search("3DFX BETA"))
		{
			envOpenGL = VOODOO ;

			// Voodoo �⡼�ɤˤĤ��Ƥ�����Ū�˻��ꤷ�ʤ���Фʤ�ʤ�
			voodoo = TRUE ;

			donotUse_multitexture = TRUE ;
		}
*/


/*
#if defined WIN32
		// Voodoo/Voodoo2 �Ǥϥե륹���꡼��⡼�ɤ��ѹ�
		if (voodoo && org_envOpenGL != VOODOO_AUTO)
		{
			if (renderer.Search("VOODOO2") ||
				version.Search("VOODOO2"))
			{
				is_set_width  = xsize = 800 ;
				is_set_height = ysize = 600 ;
			}
			else
			{
				is_set_width  = xsize = 640 ;
				is_set_height = ysize = 480 ;
			}

			full = TRUE ;
			int cw, ch ;
			SetDisplayMode(xsize, ysize, cw, ch) ;
			glutFullScreen() ;
			logprintf("Enabe full screen.\n") ;
		}
#endif	// #if defined WIN32
*/
	}

	// �٥���ޡ����⡼�ɡ��̾�⡼�ɤΤɤ���Ǥ⥻�å�
	// ®�٤˱ƶ����Фʤ��⤷����������®���ʤäƤ��ޤ����

	// PERMEDIA2 �ζػߵ�ǽ
	if (renderer.Search("PERMEDIA2"))
	{
		envOpenGL = PERMEDIA2 ;
		donotUSe_AlphaGouraud = TRUE ;
		donotUseBlend_AddSmooth = TRUE ;
		donotUseBlend_Multiply = TRUE ;
	}

	// RIVA 128/RIVA 128ZX �ζػߵ�ǽ
	if (renderer.Search("RIVA 128"))
	{
		envOpenGL = RIVA ;
		donotUSe_AlphaGouraud = TRUE ;
		donotUseBlend_AddSmooth = TRUE ;
	}


	// Savage2000
	if (renderer.Search("SAVAGE2000"))
	{
		envOpenGL = SAVAGE2000 ;
		if (!is_set_fog)
			fogFlag = FOG_MODE_SAVAGE2000 ;
	}

	// Mesa Library
	if (renderer.Search("MESA")) // && !renderer.Search("GLIDE"))
	{
		mesa = TRUE ;

		// -r -D ���ϥޥ���ƥ������㥹�ơ������򣲤˸���
		// Mesa ������ǣ��ʾ�ˤʤäƤ��ʤ���Уϣˡ�
//		if (reality >= 2 && detail)
//			limit_maxTextureUnits = 2 ;

//		if (reality >= 2 && detail)
//			donotUse_multitexture = TRUE ;
	}

	// G400 & !MESA �ʤ�ޥ���ƥ�������ػ�
	if (renderer.Search("G400") && !mesa)
	{
		donotUse_multitexture = TRUE ;
	}

	// Mesa Glide
	if (renderer.Search("MESA GLIDE"))
	{
		envOpenGL = VOODOO_MESA ;
		mesa = TRUE ;

		// Voodoo �⡼�ɤˤĤ��Ƥ�����Ū�˻��ꤷ�ʤ���Фʤ�ʤ�
		voodoo = TRUE ;

		// �����ε�ǽ��Ȥ��ȵդ�®�٤����٤��㲼���뤿�ᡢ
		// �̾�¹ԤǤ�٥���ޡ����⡼�ɤǤ⥻�å�
		donotUse_multitexture = TRUE ;
		donotUse_separate_specular_color = TRUE ;
	}

	// 3Dfx OpenGL Beta 2.1
	if (version.Search("3DFX BETA"))
	{
		envOpenGL = VOODOO ;

		// Voodoo �⡼�ɤˤĤ��Ƥ�����Ū�˻��ꤷ�ʤ���Фʤ�ʤ�
		voodoo = TRUE ;

		donotUse_multitexture = TRUE ;
		lightTwoSide = TRUE ;
	}

	// 3Dfx ICD or Q3 stand-alone
	else if (vendor.Search("3DFX") &&
			 (version.Search("STAND-ALONE") ||
			  renderer.Search("STAND-ALONE") ||
			  renderer.Search("ICD")))
	{
		envOpenGL = VOODOO_3DFX_ICD ;

		if (!isSet_zOffset)
			zOffset = Z_OFFSET_TNT_FIX ;

		donotUse_multitexture = TRUE ;

		// Voodoo �⡼�ɻ����Ĥޤ� 3Dfx stand-alone �ʤ�
		if (version.Search("STAND-ALONE") ||
			renderer.Search("STAND-ALONE"))
			voodoo = TRUE ;
	}


	// ��������ɽ���� On/Off
	// voodoo �⡼�ɡ��Ĥޤ� 3Dfx OpenGL Beta-2.1 �ޤ��� MesaGlide �ޤ��ϡ�
	// 3Dfx OpenGL Client Driver �ǡ������餬 Voodoo3 �Ǥ� Banshee �Ǥ� Rush �Ǥ�ʤ����
	if (voodoo ||
		(envOpenGL == VOODOO_3DFX_ICD &&
		 !renderer.Search("VOODOO3") && !renderer.Search("BANSHEE") && !renderer.Search("RUSH"))
		)
		drawCursor = TRUE ;

	if (voodoo)
	{
		if (!full || !is_set_width || !is_set_height)
		{
			voodoo = FALSE ;
			if (envOpenGL == VOODOO)
				drawCursor = FALSE ;
		}
	}
}


// OpenGL Extension ��Ϣ
void SetupOpenGLExtensions()
{
	// �ƥ������㡦���֥������ȴ�Ϣ
	if (!donotUse_texture_object)
		has_texture_object = IsSupported_texture_object(!donotUse_EXT_texture_object) ;
	else
		has_texture_object = NULL ;

	if (has_texture_object)
		texture_object_name = has_texture_object ;

	// �ݥꥴ�󡦥��ե��åȴ�Ϣ
	if (!donotUse_polygon_offset)
		has_polygon_offset = IsSupported_polygon_offset(!donotUse_EXT_polygon_offset) ;
	else
		has_polygon_offset = NULL ;

	if (has_polygon_offset)
		polygon_offset_name = has_polygon_offset ;

	// �ե������Ķ��ޥå��� texgen_reflection ��Ϣ
	if (!donotUse_texgen_reflection)
		has_texgen_reflection = IsSupported_texgen_reflection(!donotUse_ARB_texture_cube_map,
															  !donotUse_EXT_texture_cube_map,
															  !donotUse_NV_texgen_reflection) ;
	else
		has_texgen_reflection = NULL ;

	if (has_texgen_reflection)
		texgen_reflection_name = has_texgen_reflection ;


	///////////////////////////
//	MessageBox(NULL, texture_object_name, _T("tecture object supported"), MB_OK | MB_ICONSTOP) ;


	// �ޥ���ƥ��������Ϣ�ʥƥ������㡦���֥�������ɬ�ܡ�
	if (!donotUse_multitexture && has_texture_object)
		has_multitexture = IsSupported_multitexture(!donotUse_ARB_multitexture,
													!donotUse_EXT_multitexture,
													!donotUse_SGIS_multitexture) ;
	else
		has_multitexture = NULL ;


	CheckOpenGLError() ;

	if (has_multitexture)
		multitexture_name = has_multitexture ;
	maxTextureUnits = GetMaxTextureUnits() ;
	if (limit_maxTextureUnits >= 2 && maxTextureUnits > limit_maxTextureUnits)
		maxTextureUnits = limit_maxTextureUnits ;


	// separate_specular_color ��Ϣ
	if (!donotUse_separate_specular_color)
		has_separate_specular_color = IsSupported_separate_specular_color(!donotUse_ARB_separate_specular_color,
																		  !donotUse_EXT_separate_specular_color) ;
	else
		has_separate_specular_color = NULL ;

	if (has_separate_specular_color)
		separate_specular_color_name = has_separate_specular_color ;


	// texture_filter_anisotropic ��Ϣ
	if (!donotUse_texture_filter_anisotropic)
		has_texture_filter_anisotropic = IsSupported_texture_filter_anisotropic(!donotUse_EXT_texture_filter_anisotropic) ;
	else
		has_texture_filter_anisotropic = NULL ;

	if (has_texture_filter_anisotropic)
		texture_filter_anisotropic_name = has_texture_filter_anisotropic ;


	// texture_compression ��Ϣ
	if (!donotUse_texture_compression)
		has_texture_compression_Enum = IsSupported_texture_compression_Enum(!donotUse_ARB_texture_compression) ;
	else
		has_texture_compression_Enum = NULL ;

	if (has_texture_compression_Enum)
		texture_compression_Enum_name = has_texture_compression_Enum ;

	// texture_compression_s3tc ��Ϣ
	if (!donotUse_texture_compression_s3tc)
		has_texture_compression_s3tc = IsSupported_texture_compression_s3tc(!donotUse_EXT_texture_compression_s3tc) ;
	else
		has_texture_compression_s3tc = NULL ;

	if (has_texture_compression_s3tc)
		texture_compression_s3tc_name = has_texture_compression_s3tc ;

	// texture_compression_FXT1 ��Ϣ
	if (!donotUse_texture_compression_FXT1)
		has_texture_compression_FXT1 = IsSupported_texture_compression_FXT1(!donotUse_3DFX_texture_compression_FXT1) ;
	else
		has_texture_compression_FXT1 = NULL ;

	if (has_texture_compression_FXT1)
		texture_compression_FXT1_name = has_texture_compression_FXT1 ;


	CheckOpenGLError() ;
	maxTextureMaxAnisotropy = GetMaxTextureMaxAnisotropy() ;
}


String MakeTextureInformationsLog()
{
	String texInfoLog ;

	if (!solid)
	{
		if (crystal == 1)
			texInfoLog += "Double sided transparency sphere mapping without z-buffer.\n" ;
		else
		{
			switch(texture)
			{
			case 1:
				{
					if (!detail)
						texInfoLog += "Per-texel Sphere mapped specular hi-lights.\n" ;
					else
						texInfoLog += "Detail mapping with per-texel sphere mapped specular hi-lights (max 1-pass 2-texturing).\n" ;
				}
				break ;
			case 2:
				{
					if (!detail)
						texInfoLog += "Per-vertex shading with per-texel sphere mapped specular hi-lights (1-pass rendering).\n" ;
					else
						texInfoLog += "Per-vertex shaded detail mapping with per-texel sphere mapped specular hi-lights (max 1-pass 2-texturing).\n" ;
				}
				break ;

//			case 3:
//				texInfoLog += "Per-vertex shading with per-texel sphere mapped specular hi-lights (2-pass rendering).\n" ; break ;
			}
		}
	}
	else if (!reality)
	{
		switch(mode_solid)
		{
		case 1:
			texInfoLog += "Solid (plane projection) mapping.\n" ; break ;
		case 2:
			texInfoLog += "Per-vertex shaded solid mapping (single color blending).\n" ; break ;
		case 3:
			texInfoLog += "Per-vertex shaded solid mapping (2-pass separate specular color blending).\n" ; break ;
		case 13:
			texInfoLog += "Per-vertex shaded solid mapping (1-pass separate specular color blending).\n" ; break ;
		}
	}
	else
	{
		switch (reality)
		{
		case 1:
			if (!detail)
				texInfoLog += "Solid mapping with per-texel sphere mapped specular (max 2-pass 2-texturing).\n" ;
			else
				texInfoLog += "Detail solid mapping with per-texel sphere mapped specular (max 3-pass 3-texturing).\n" ;
			break ;

		case 2:
			if (!detail)
				texInfoLog += "Diffuse shaded solid mapping with per-texel sphere mapped specular (max 2-pass 2-texturing).\n" ;
			else
				texInfoLog += "Diffuse shaded and detail solid mapping with per-texel sphere mapped specular (max 3-pass 3-texturing).\n" ;
			break ;
//		case 3:
//			texInfoLog += "Diffuse shaded solid mapping and specular shading environment mapping.\n" ; break ;

		case 11:
		case 21:
			if (!detail)
				texInfoLog += "Solid mapping with per-texel sphere mapped specular hi-lights (max 1-pass 2-texturing).\n" ;
			else
			{
				if (detail == 2)
					texInfoLog += "Detail solid mapping with per-texel sphere mapped specular (max 2-pass 3-texturing).\n" ;
				else	// detail == 3
					texInfoLog += "Detail solid mapping with per-texel sphere mapped specular (max 1-pass 3-texturing).\n" ;
			}
			break ;

		case 12:
		case 22:
			if (!detail)
				texInfoLog += "Diffuse shaded solid mapping with per-texel sphere mapped specular (max 1-pass 2-texturing).\n" ;
			else
			{
				if (detail == 2)
					texInfoLog += "Diffuse shaded detail solid mapping with per-texel sphere mapped specular (max 2-pass 3-texturing).\n" ;
				else	// detail == 3
					texInfoLog += "Diffuse shaded detail solid mapping with per-texel sphere mapped specular (max 1-pass 3-texturing).\n" ;
			}
			break ;
		}
	}


	// �ƥ������㡦�ե��륿
	texInfoLog += TextureFilteringLog() + ".\n" ;

	// ���󥢥����ȥ��ԥå��ե��륿��󥰡ʥƥ���������ѻ��Τߡ�
	if (maxAnisotropy > 1.0f)
	{
		texInfoLog.AddFormat("Enable " + texture_filter_anisotropic_name + ".\n") ;
		texInfoLog += "Texture level " + IntToString((int)maxAnisotropy) + " anisotropic filtering.\n" ;
	}

	return texInfoLog ;
}


String MakeOpenGLClientInformationsLog()
{
	String glInfoLog ;

	// OpenGL ���饤����ȡʥ���ѥ�����˾���
	glInfoLog = "OpenGL client (compile-time) informations.\n" ;

#if !defined GL_VERSION_1_1 && !defined GL_VERSION_1_2
	glInfoLog += "  Compiled base: OpenGL-1.0\n" ;
#elif !defined GL_VERSION_1_2	// OpenGL-1.1
	glInfoLog += "  Compiled base: OpenGL-1.1\n" ;
#else	// defined GL_VERSION_1_2
	glInfoLog += "  Compiled base: OpenGL-1.2\n" ;
#endif

	// ���ݡ��Ȳ�ǽ��ǽ�ʥС�������¸��ǽ��
	glInfoLog += "\n  Supportable version dependence capabilities (depend on run-time OpenGL version):\n" ;

#ifdef GL_VERSION_1_1
	// OpenGL-1.1
	glInfoLog += "\tOpenGL-1.1 Texture Object\t ... -e, -C, -r\n" ;
	glInfoLog += "\tOpenGL-1.1 Polygon Offset\t ... -zoffset with -s 3, -r, -D\n" ;
#endif	// #ifdef GL_VERSION_1_1

	// OpenGL-1.2
	// �������ѥ�᥿�Τߤ�������ɤ����ᡢ�����ФΤߤΥ��ݡ��Ȥ� OK
	glInfoLog += "\tOpenGL-1.2 Separate Specular Color\t ... -s 3\n" ;

	// ���ݡ��Ȳ�ǽ��ĥ��ǽ
	glInfoLog += "\n  Supportable extensions (depend on run-time OpenGL extensions):\n" ;

#ifdef GL_ARB_multitexture
	glInfoLog += "\tGL_ARB_multitexture\t\t ... -r 2\n" ;
#endif
#ifdef GL_EXT_multitexture
	glInfoLog += "\tGL_EXT_multitexture\t\t ... -r 2\n" ;
#endif
#ifdef GL_SGIS_multitexture
	glInfoLog += "\tGL_SGIS_multitexture\t\t ... -r 2\n" ;
#endif

#ifdef GL_ARB_separate_specular_color
	glInfoLog += "\tGL_ARB_separate_specular_color\t ... -s 3\n" ;
#endif
#ifdef GL_EXT_separate_specular_color
	glInfoLog += "\tGL_EXT_separate_specular_color\t ... -s 3\n" ;
#endif

#ifdef GL_EXT_texture_object
	glInfoLog += "\tGL_EXT_texture_object\t\t ... -e, -C, -r\n" ;
#endif

#ifdef GL_EXT_polygon_offset
	glInfoLog += "\tGL_EXT_polygon_offset\t\t ... -zoffset with -s 3, -r, -D\n" ;
#endif

#ifdef GL_ARB_texture_cube_map
	glInfoLog += "\tGL_ARB_texture_cube_map\t\t ... -FAKE with -e, -C, -r\n" ;
#endif
#ifdef GL_EXT_texture_cube_map
	glInfoLog += "\tGL_EXT_texture_cube_map\t\t ... -FAKE with -e, -C, -r\n" ;
#endif
#ifdef GL_NV_texgen_reflection
	glInfoLog += "\tGL_NV_texgen_reflection\t\t ... -FAKE with -e, -C, -r\n" ;
#endif

#ifdef GL_EXT_texture_filter_anisotropic
	glInfoLog += "\tGL_EXT_texture_filter_anisotropic\t ... -anisotropy with -e, -s, -C, -r\n" ;
#endif

#ifdef GL_ARB_texture_compression
	glInfoLog += "\tGL_ARB_texture_compression\t ... -TC or -tformat with -s, -e, -C, -r\n" ;
#endif

#ifdef GL_EXT_texture_compression_s3tc
	glInfoLog += "\tGL_EXT_texture_compression_s3tc\t ... -S3TC or -tformat with -s, -e, -C, -r\n" ;
#endif

#ifdef GL_3DFX_texture_compression_FXT1
	glInfoLog += "\tGL_3DFX_texture_compression_FXT1\t ... -FXT1 or -tformat with -s, -e, -C, -r\n" ;
#endif

	glInfoLog += "\n" ;
	return glInfoLog ;
}


String MakeOpenGLServerInformationsLog()
{
	String glInfoLog ;

	// OpenGL �����Сʥ�󥿥���˾���
	glInfoLog = "OpenGL server (run-time) informations.\n" ;

	glInfoLog += String("  Vendor    : ") + glVendor + "\n" ;
	glInfoLog += String("  Renderer  : ") + glRenderer + "\n" ;
	glInfoLog += String("  Version   : ") + glVersion + "\n" ;
	glInfoLog += String("  Extensions:\n") + glExtensions + "\n" ;

	glInfoLog += "\n" ;
	glInfoLog += "  Max texture size: " + IntToString(maxTextureSize) + "x" + IntToString(maxTextureSize) + "\n" ;
//	glInfoLog += "  Texture internal format: " + internalFormatSymbol + "\n" ;
	if (internalFormatInfos != "")
		glInfoLog += "  Texture internal format: " + internalFormatSymbol + " (" + internalFormatInfos + ")\n" ;
	else
		glInfoLog += "  Texture internal format: " + internalFormatSymbol + "\n" ;

	// ���Ѷػߥ��ץ����
	String prohibit ;

	if (donotuse_OpenGL_1_1)		prohibit += "\tAll of OpenGL-1.1 or later\n" ;
	else if (donotuse_OpenGL_1_2)	prohibit += "\tAll of OpenGL-1.2 or later\n" ;

	// �ޥ���ƥ��������OpenGL-1.1 �ʹ�ɬ�ܡ�
	if (!donotuse_OpenGL_1_1)
	{
		if (donotUse_multitexture) prohibit += "\tMulti-Texture\n" ;
		else
		{
			if (donotUse_ARB_multitexture) prohibit += "\tGL_ARB_multitexture\n" ;
			if (donotUse_EXT_multitexture) prohibit += "\tGL_EXT_multitexture\n" ;
			if (donotUse_SGIS_multitexture) prohibit += "\tGL_SGIS_multitexture\n" ;
		}
	}

	// OpenGL-1.2 ��°����ǽ
	if (!donotuse_OpenGL_1_2)
	{
		// ���ѥ졼�ȡ����ڥ���顼
		if (donotUse_separate_specular_color) prohibit += "\tSeparate Specular Color\n" ;
		else
		{
			if (donotUse_ARB_separate_specular_color) prohibit += "\tGL_ARB_separate_specular_color\n" ;
			if (donotUse_EXT_separate_specular_color) prohibit += "\tGL_EXT_separate_specular_color\n" ;
		}
	}

	// OpenGL-1.1 ��°����ǽ
	if (!donotuse_OpenGL_1_1)
	{
		// �ƥ������㡦���֥�������
		if (donotUse_texture_object) prohibit += "\tTexture Object\n" ;
		else
		{
			if (donotUse_EXT_texture_object) prohibit += "\tGL_EXT_texture_object\n" ;
		}
		// �ݥꥴ�󡦥��ե��å�
		if (donotUse_polygon_offset) prohibit += "\tPolygon Offset\n" ;
		else
		{
			if (donotUse_EXT_polygon_offset) prohibit += "\tGL_EXT_polygon_offset\n" ;
		}
	}

	// �ƥ������㰵�̥ե����ޥåȡ�OpenGL-1.1 �ʹ�ɬ�ܡ�
	if (!donotuse_OpenGL_1_1)
	{
		if (donotUse_texture_compression) prohibit += "\tTexture Compression\n" ;
		else
		{
			if (donotUse_ARB_texture_compression)       prohibit += "\tGL_ARB_texture_compression\n" ;
			if (donotUse_EXT_texture_compression_s3tc)  prohibit += "\tGL_EXT_texture_compression_s3tc\n" ;
			if (donotUse_3DFX_texture_compression_FXT1) prohibit += "\tGL_3DFX_texture_compression_FXT1\n" ;
		}
	}

	// OpenGL �С������Ȥ�̵�ط��ε�ǽ
	// �ƥ��������ɸ����
	if (donotUse_texgen_reflection) prohibit += "\tTex-Gen Reflection\n" ;
	else
	{
		if (donotUse_ARB_texture_cube_map) prohibit += "\tGL_ARB_texture_cube_map\n" ;
		if (donotUse_EXT_texture_cube_map) prohibit += "\tGL_EXT_texture_cube_map\n" ;
		if (donotUse_NV_texgen_reflection) prohibit += "\tGL_NV_texgen_reflection\n" ;
	}

	// ����ե�������
	if (donotUSe_AlphaGouraud) prohibit += "\tAlpha-Gouraud\n" ;

	// �֥��ɥե�����
	if (donotUseBlend_AddSmooth) prohibit += "\tBlend-Func Add-Smooth (GL_ONE, GL_ONE_MINUS_SRC_COLOR)\n" ;
	if (donotUseBlend_Multiply)  prohibit += "\tBlend-Func Modulate (GL_ZERO, GL_SRC_COLOR) and (GL_DST_COLOR, GL_ZERO)\n" ;

	if (prohibit == "")
		prohibit = " None\n" ;
	else
		prohibit = "\n" + prohibit + "\n" ;

	// �ػߵ�ǽ��ޤȤ�ƥ������ɲ�
	glInfoLog += "\nProhibited capabilities:" + prohibit ;

	// �ºݤ˻��Ѳ�ǽ�ʵ�ǽ
//	glInfoLog += "Setting:\n" ;
	if (has_multitexture)				glInfoLog.AddFormat(multitexture_name + " (max texture units: %d) is available.\n", maxTextureUnits) ;
	if (has_separate_specular_color)	glInfoLog.AddFormat(separate_specular_color_name + " is available.\n") ;
	if (has_texture_object)				glInfoLog.AddFormat(texture_object_name + " is available.\n") ;
	if (has_polygon_offset)				glInfoLog.AddFormat(polygon_offset_name + " is available.\n") ;
	if (has_texgen_reflection)			glInfoLog.AddFormat(texgen_reflection_name + " is available.\n") ;
	if (has_texture_filter_anisotropic)	glInfoLog.AddFormat(texture_filter_anisotropic_name + " (max of max anisotropy: %3.1f) is available.\n", maxTextureMaxAnisotropy) ;
	if (has_texture_compression_Enum)	glInfoLog.AddFormat(texture_compression_Enum_name + " is available.\n") ;
	if (has_texture_compression_s3tc)	glInfoLog.AddFormat(texture_compression_s3tc_name + " is available.\n") ;
	if (has_texture_compression_FXT1)	glInfoLog.AddFormat(texture_compression_FXT1_name + " is available.\n") ;

	return glInfoLog ;
}

/*
String MakeOpenGLInformationsLog()
{
	return (MakeOpenGLClientInformationsLog() + MakeOpenGLServerInformationsLog()) ;
}
*/

String MakeViewportLog()
{
	String log ;
	log.Format("Viewport size: %d x %d\n", xsize, ysize) ;
	return log ;
}

String MakeColorBufferInfoLog()
{
	String log ;

/*
	if (glut_gameMode)
	{
		GLint viewport[4] ;
		glGetIntegerv(GL_VIEWPORT, viewport) ;
		xsize = viewport[2] ;
		ysize = viewport[3] ;
	}
*/

	if (glut_gameMode)
	{
		log = "Color buffer: GLUT Game Mode " + glut_gameModeString ;
	}
	else
		log.Format("Color buffer: %d x %d", xsize, ysize) ;

	GLint r, g, b, a, d ;
	glGetIntegerv(GL_RED_BITS,   &r) ;
	glGetIntegerv(GL_GREEN_BITS, &g) ;
	glGetIntegerv(GL_BLUE_BITS,  &b) ;
	glGetIntegerv(GL_ALPHA_BITS, &a) ;
	glGetIntegerv(GL_DEPTH_BITS, &d) ;

//	int rgba ;
//	rgba = r + g + b + a ;

	if (a)
//		log.AddFormat(" x (%dbpp (R:%d G:%d B:%d A:%d) + Z:%d)\n", rgba, r, g, b, a, d) ;
		log.AddFormat(" (R:%d G:%d B:%d A:%d + Z:%d)\n", (int)r, (int)g, (int)b, (int)a, (int)d) ;
	else
//		log.AddFormat(" x (%dbpp (R:%d G:%d B:%d) + Z:%d)\n", rgba, r, g, b, d) ;
		log.AddFormat(" (R:%d G:%d B:%d + Z:%d)\n", (int)r, (int)g, (int)b, (int)d) ;

	return log ;
}


void SetStaticRenderingFlags()
{
	flag_withoutAccumBufferSuperSampling = WITHOUT_ACCUM_BUFFER_SUPER_SAMPLING ;
	flag_additiveAlphaSuperSampling = ADDITIVE_ALPHA_SUPER_SAMPLING ;
	flag_glNormal = FLAG_GL_NORMAL ;
	flag_glassTexCoord = FLAG_GLASS_TEX_COORD ;
	flag_shadeTexCoord = FLAG_SHADE_TEX_COORD ;
	flag_shadeTexCoordOnlySolid = FLAG_SHADE_TEX_COORD_ONLY_SOLID ;
	flag_modulateTexCoord = FLAG_MODULATE_TEX_COORD ;

	if (mode_normal || mode_solid >= 2 || mode_env >= 2)
		specularFlag = 1.0f ;
	else if (mode_real == 3 || mode_real == 13)
//		specularFlag = 0.4f ;
		specularFlag = 0.0f ;
	else
		specular = 0.0f ;
}


// ����⡼�ɼ��̻�
void SetClockRenderMode()
{
	// ���ꥢ
	mode_normal = 0 ;
	mode_solid  = 0 ;
	mode_env    = 0 ;
	mode_real   = 0 ;


	// "-DETAILTEX" �� texture_object ɬ��
	if (!has_texture_object)
	{
		detail = FALSE ;

		if (env == 2)
		{
			env = 1 ;
			texture = 1 ;
		}
	}

	if (donotUseBlend_Multiply && (detail == 1 || (reality == 2 && detail) || (reality == 3 && detail)))
		detail = FALSE ;


	// -r 2 ���ץ������ѻ������餫�Υޥ���ƥ������㤬���Ѳ�ǽ�ʾ��
	if (has_multitexture && reality >= 2 /* && maxTextureUnits >= 2 */)	// -r 1 ��̤�б�
	{
		reality += 10 ;
		logprintf("Use " + multitexture_name + ".\n") ;

		// �ޥ���ƥ������㥹�ڥ���顼�⡼�ɤǤϡ����֥���ǡ��������ޤ���
		specularTimeGradRatio = SPECULAR_TIME_GRAD_RATIO_TEX_ENV ;

		// �ޥ���ƥ���������ѻ��ϡ�ADD�ϻ����Բ�
		// ����������Ū�ˤϼ����β�ǽ������
		specularBlend = SPECULAR_BLEND_ADD_SMOOTH ;

		if (detail)
		{
			if (maxTextureUnits >= 3)	// ���ʰʾ�Υޥ���ƥ���������б����Ƥ�����
				detail = 3 ;	// 1 path 3 texture
			else
				detail = 2 ;	// 2 path 3 texture(2 + 1)
		}
	}
	else if (detail)
	{
			detail = 1 ;	// 3 path 3 texture(1 + 1 + 1)
	}

	if (env)
	{
		// �ƥ������㥹�ڥ���顼�⡼�ɤǤϡ����֥���ǡ��������ޤ���
		specularTimeGradRatio = SPECULAR_TIME_GRAD_RATIO_TEX_ENV ;
	}

	if (detail && env)
	{
		if (has_multitexture && detail && env) // == 2)
		{
			// -env ? -D �ϥޥ���ƥ�������ɬ��
			logprintf("Use " + multitexture_name + ".\n") ;

			if (env == 1)
				detail = 4 ;	// -env 1 1-pass 2 texture
			else
				detail = 5 ;	// -env 2 1-pass 2 texture
		}
		else
			detail = FALSE ;
	}

	if (detail)
	{
		// detail �ƥ�������Υƥ��������ɸ��
		dsAxis = cdsAxis ;
		dtAxis = cdtAxis ;

		dsAxis.Unit() ;
		dtAxis.Unit() ;

		if (dtex_scale == IS_SET_DETAIL_TEX_SCALE)
		{
			if (reality)
				dtex_scale = DETAIL_TEX_SCALE_REAL ;
			else if (env)
				dtex_scale = DETAIL_TEX_SCALE_ENV ;
		}

		dsAxis /= dtex_scale ;
		dtAxis /= dtex_scale ;

		vdsAxis = dsAxis.v ;
		vdtAxis = dtAxis.v ;

		// �ǥ��ƥ�����ƥ������㥬�������
		if (detailMapGamma == IS_SET_DETAIL_MAP_GAMMA)
		{
			if (reality)
				detailMapGamma = DETAIL_MAP_GAMMA_REAL ;
			else if (env == 1)
				detailMapGamma = DETAIL_MAP_GAMMA_ENV1 ;
			else if (env == 2)
				detailMapGamma = DETAIL_MAP_GAMMA_ENV2 ;
		}

		// �ǥ��ƥ�����ǥե���ȥƥ�������ե�����
		if (detail_file == IS_SET_DETAIL_FILE)
		{
			if (reality)
				detail_file = DETAIL_FILE_REAL ;
			else if (env)
				detail_file = DETAIL_FILE_ENV ;
		}
	}
	else
	{
		// detail �ʤ����ϡ����٤� NULL
		vdsAxis = NULL ;
		vdtAxis = NULL ;
	}

	switch (texture)
	{
	case 0:	// �Ρ��ޥ륷�����ǥ��󥰡ʥƥ�������ʤ���
		mode_normal = 1 ;
		break ;

	case 1:
		if (solid) mode_solid = 1 ;
		else       mode_env   = 1 ;
		break ;

	case 2:
		if (solid) mode_solid = 2 ;
		else
		{
			mode_env   = 2 ;
			// �ƥ�������ϥ��饤�ȥ֥����ѻ��֥���ǡ������Ψ
//			specularTimeGradRatio = SPECULAR_TIME_GRAD_RATIO_TEX_ENV ;
		}
		break ;

	case 3:
		if (crystal)      mode_crystal = 1 ;
		else if (!solid)  mode_env     = 3 ;
		else if (reality) mode_real    = reality ;	// 1(11) or 2(12) or 3(13)
		else              mode_solid   = 3 ;	// 3 or 13
	}


	if (!mode_normal && !mode_solid && has_texture_object)
	{
		// �ƥ������㡦���֥������Ȼ���
		logprintf("Use " + texture_object_name + ".\n") ;
	}

	// -s 3 ���ץ������ѻ������餫�� separate_specular_color �����Ѳ�ǽ�ʾ��
	// texture, solid �ˤ��Ѳ��ʤ�
	if (has_separate_specular_color && mode_solid == 3)
	{
		mode_solid += 10 ;	// -s 3 ... 13
		logprintf("Enable " + separate_specular_color_name + ".\n") ;
	}

	if (mode_solid == 1 || mode_solid == 3 || mode_real) specular = 0.0f ;
	else                                                 specular = 1.0f ;

	// -s 3 �⤷���� -r �ǥޥ���ѥ�����-z �����ꤵ��Ƥ��ꡢ���ĥݥꥴ�󥪥ե��åȻ��Ѳ�ǽ�ʤ��
	if ((mode_solid == 3 || mode_real >= 1 || mode_real <= 3) && zOffset > 0.0f && has_polygon_offset)
	{
		// �ݥꥴ�󡦥��ե��åȻ���
		polygon_offset = TRUE ;
		logprintf("Use " + polygon_offset_name + ".\n") ;
	}
	else
	{
		zOffset = 0.0f ;
		polygon_offset = FALSE ;
	}

	// �ե������Ķ��ޥå�
	if (has_texgen_reflection && fakeSphereMap && (mode_env || mode_crystal || mode_real))
	{
		logprintf("Enable " + texgen_reflection_name + " for fake sphere mapping.\n") ;
	}
	else
	{
		fakeSphereMap = FALSE ;
	}

	// Texture Compression ��Ϣ
	if (internalFormatSymbol / "COMPRESSED")
	{
		if (internalFormatSymbol / "S3TC" && has_texture_compression_s3tc)
			logprintf("Enable " + texture_compression_Enum_name + " / " + texture_compression_s3tc_name + ".\n") ;
		else if (internalFormatSymbol / "FXT1" && has_texture_compression_FXT1)
			logprintf("Enable " + texture_compression_Enum_name + " / " + texture_compression_FXT1_name + ".\n") ;
		else
			logprintf("Enable " + texture_compression_Enum_name + ".\n") ;
	}

	// ����åɥޥåԥ��ѥƥ������㥹�����륻�å�
	// �ƥ������㥪�֥������Ȥ��Ȥ��� -r ���ץ������ϡ��ƥ�������Υ������������
	if (!has_texture_object && mode_real)
	{
		if (s_scale < TEX_SCALE)
			s_scale = TEX_SCALE ;
		if (t_scale < TEX_SCALE)
			t_scale = TEX_SCALE ;
	}

	if (orgImageFlag)
	{
		sAxis = csAxis_orgImage ;
		tAxis = ctAxis_orgImage ;
	}
	else
	{
		sAxis = csAxis ;
		tAxis = ctAxis ;
	}
	sAxis.Unit() ;
	tAxis.Unit() ;
	sAxis /= s_scale ;
	tAxis /= t_scale ;



	// Specular mapping default blending mode
	if (donotUseBlend_AddSmooth && (mode_real == 1 || mode_real == 2 || mode_real == 3 || crystal))
	{
//		if (specularBlend == SPECULAR_BLEND || specularBlend > 1)
			specularBlend = SPECULAR_BLEND_ADD ;
	}

	if (crystal && specularBlend == SPECULAR_BLEND)
		specularBlend = SPECULAR_BLEND_ADD ;

	if (specularBlend == SPECULAR_BLEND || specularBlend > 1)
		specularBlend = SPECULAR_BLEND_ADD_SMOOTH ;


	// Detail texture default blending mode
	if (detailBlend == DETAIL_BLEND || detailBlend > 1)
		detailBlend = DETAIL_BLEND_MULTIPLY_DST_BASE ;


	// ���饹�����ڥ���顼�ο���
	// �֥��ɥ⡼�����ˤ�ä��Ѳ�
	if (specularBlend == SPECULAR_BLEND_ADD)
	{
		// ADD
		// GL_ONE, GL_ONE ���ѻ�
		addSrcBlendFactor = GL_ONE ;
		addDstBlendFactor = GL_ONE ;
		addSrcBlendFactorGlass = GL_ONE ;
		addDstBlendFactorGlass = GL_ONE ;

		if (mode_crystal)
			realitySpecularBright = CRYSTAL_SPECULAR_ADD ;
		else
			realitySpecularBright = REALITY_SPECULAR ;

		if (reflection)
			envGlassBright = ENV_GLASS_BRIGHT_REF ;
		else
		{
			if (mode_real)
				envGlassBright = ENV_GLASS_BRIGHT_R ;
			else
				envGlassBright = ENV_GLASS_BRIGHT ;
		}
	}
	else	// if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)
	{
		// GL_ONE_MINUS_DST_COLOR, GL_ONE ���ѻ�
		addSrcBlendFactor = GL_ONE ; //GL_ONE_MINUS_DST_COLOR ;
		addDstBlendFactor = GL_ONE_MINUS_SRC_COLOR ;

		// Add �ǤϤʤ���硢���֥���ǡ��������ޤ���
		if (mode_real)
			specularTimeGradRatio *= SPECULAR_TIME_GRAD_RATIO_REAL ;
		if (mode_env)
			specularTimeGradRatio *= SPECULAR_TIME_GRAD_RATIO_ENV2 ;

		if (mode_crystal)
		{
			addSrcBlendFactorGlass = GL_ONE ;
			addDstBlendFactorGlass = GL_ONE_MINUS_SRC_COLOR ;
			realitySpecularBright = REALITY_SPECULAR_ONE_MINUS_COLOR ;

			if (reflection)
				envGlassBright = ENV_GLASS_BRIGHT_REF_ONE_MINUS_COLOR ;
			else
				envGlassBright = ENV_GLASS_BRIGHT_ONE_MINUS_COLOR ;
		}
		else
		{
			// ���ꥹ����ʳ��ϡ����饹�ϲû��Τޤޤ������ɤ���
			addSrcBlendFactorGlass = GL_ONE ;
			addDstBlendFactorGlass = GL_ONE ;
			realitySpecularBright = REALITY_SPECULAR_ONE_MINUS_COLOR ;

			if (reflection)
				envGlassBright = ENV_GLASS_BRIGHT_REF ;
			else
			{
				if (mode_real)
					envGlassBright = ENV_GLASS_BRIGHT_R ;
				else if (mode_crystal)
					envGlassBright = ENV_GLASS_BRIGHT * CRYSTAL_GLASS_ADD_BRIGHT_RATIO ;
				else
					envGlassBright = ENV_GLASS_BRIGHT ;
			}
		}
	}


	if (mode_real || mode_crystal)
	{
		if (specularBlend == SPECULAR_BLEND_ADD)
		{
			logprintf("Specular-texture blend: Add\n") ;
//			if (mode_real >= 10)

			logprintf("\tblend-func (src: GL_ONE, dst: GL_ONE)\n") ;
		}
		else // if (specularBlend == SPECULAR_BLEND_ADD_SMOOTH)
		{
			logprintf("Specular-texture blend: Add-Smooth\n") ;

			if (mode_real >= 10)
			{
				logprintf("\ttex-env (2nd-stage texture env mode: GL_BLEND)\n") ;
				logprintf("\tblend-func (src: GL_ONE, dst: GL_ONE_MINUS_SRC_COLOR)\n") ;
			}
			else
				logprintf("\tblend-func (src: GL_ONE, dst: GL_ONE_MINUS_SRC_COLOR)\n") ;
		}
	}


	if (detail)
	{
		logprintf("Detail-texture blend: Modulate\n") ;

		if (mode_real)
		{
			if (detail == 1 || (detail && (mode_real == 11 || mode_real == 1)))
			{
				if (detailBlend == DETAIL_BLEND_MULTIPLY_DST_BASE)
					logprintf("\tblend-func (src: GL_ZERO, dst: GL_SRC_COLOR)\n") ;

				else	// if (detailBlend == DETAIL_BLEND_MULTIPLY_SRC_BASE)
					logprintf("\tblend-func (src: GL_DST_COLOR, dst: GL_ZERO)\n") ;
			}
			else
				logprintf("\ttex-env (2nd-stage texture env mode: GL_MODULATE)\n") ;
		}
		else if (mode_env)	// -e ? -D with multitex
		{
			logprintf("\ttex-env (1st-stage texture env mode: GL_MODULATE)\n") ;
		}
	}

	if (detailBlend == DETAIL_BLEND_MULTIPLY_DST_BASE)
	{
		multiSrcBlendFactor = GL_ZERO ;
		multiDstBlendFactor = GL_SRC_COLOR ;
	}
	else	// if (detailBlend == DETAIL_BLEND_MULTIPLY_SRC_BASE)
	{
		multiSrcBlendFactor = GL_DST_COLOR ;
		multiDstBlendFactor = GL_ZERO ;
	}


	// �Ρ��ޥ� �ޤ��� -solid 2, 3 �ʳ��λ��ס�-s 1, -e, -r, -C�ˤ�
	// ���פ�ϻ�ѷ��ʲ��ξ��� -glass 1 �ϻ����Բ�
	if (clock_glass == 1 &&
		((mode_normal && !light) ||
		  mode_solid == 1 ||
		  mode_env ||
		  mode_real ||
		  mode_crystal ||
		  clock_div <= 6)
		)
		clock_glass = 2 ;


	// -lr �ե饰���ǥե���ȡ�-1�ˤξ��
	// �Ρ��ޥ롢-solid 2, 3 �Ǥ� ON������ʳ��� OFF ��
	if (rotateLight == ROTATE_LIGHT)
	{
		if (mode_normal || mode_solid >= 2)
			rotateLight = TRUE ;
		else
			rotateLight = FALSE ;
	}
	else if (rotateLight)
		rotateLight = TRUE ;

//	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize) ;	// �ƥ�������κ��祵����


	// ���󥢥����ȥ��ԥå��ե��륿���
	if (maxTextureMaxAnisotropy <= 1.0f || !texture)
	{
		maxAnisotropy = default_maxAnisotropy = 1.0f ;
	}
	else
	{
		if (default_maxAnisotropy == NONE)	// �ǥե���ȡ�-ani ���ץ���󤬻��ꤵ��Ƥ��ʤ���� OFF��
		{
			maxAnisotropy = 1.0f ;
			default_maxAnisotropy = maxTextureMaxAnisotropy ;
		}
		else
		{
			// -ani -1 �Τ褦�˥ޥ��ʥ������ꤵ�줿���ϲ�ǽ�ʺ����ͤ�ǥե���Ȥ˥��å�
//			if (default_maxAnisotropy < 0.0f)
			if (default_maxAnisotropy < 0.0f || default_maxAnisotropy > maxTextureMaxAnisotropy)
				default_maxAnisotropy = maxTextureMaxAnisotropy ;

			// -ani ���ץ���󤬻��ꤵ��Ƥ����������֤� ON
			maxAnisotropy = default_maxAnisotropy ;
		}

		UpdateAnisotropy(maxAnisotropy) ;
	}

	if (texture)
		texInformationLog = MakeTextureInformationsLog() ;
	else
		texInformationLog = "Per-vertex shading.\n" ;

	if ((mode_normal && light) || mode_solid >= 2 || mode_env >= 2 || (mode_real >= 2 && mode_real != 11))
		vertexShadingFlag = TRUE ;
	else
		vertexShadingFlag = FALSE ;

	if (g_smooth < 0)	// -gsmooth -1�ʥǥե���ȡˤξ��ϼ�ưĴ��
		g_smooth = texture ;

	SetStaticRenderingFlags() ;

	logprintf(texInformationLog) ;
}


// ���ץ����OpenGL �Ķ�����
// ���פ�������ݥꥴ�����׻�
// �����������饹�Ͻ���
int CalcClockTriangles()
{
	int nTriangles ;
	int nShellTri ;
//	int nShellCullTri ;
	int nGoldenTri ;
	int nBoardTri ;
	int nRoofTri ;
	int nHedronTri ;
	int nNeedleTri ;
	int nDetailTri ;

	// ���ܹ�����
	nShellTri = (n_s_vtx[clock_mesh] - 2) * clock_div * 2	// ʢ��ʬ�ʻ��շ��ݥꥴ���
			  + clock_div ;									// ��κǸ����ʬ

//	nShellTri = (n_s_vtx[clock_mesh] - 2) * clock_div * 2 ;	// ʢ��ʬ�ʻ��շ��ݥꥴ���

	if (gold_depth >= 0)
		nGoldenTri = Pow(4, gold_depth + 1) * n_golden ;	// �Ƶ����١ʣ��Σ�ܣ���ݥꥴ��ˡ߻��פγѿ��ˤ���Ѳ�
	else
		nGoldenTri = 0 ;

	if (!lid)
		nRoofTri = (n_r_vtx[clock_mesh] - 2) * clock_div * 2	// ŷ��ʳ��ʻ��շ��ݥꥴ����ʬ��
				 + clock_div ;									// ŷ��κǽ����ʬ
	else
		nRoofTri = 0 ;

	if (!cullObj)
	{
		nBoardTri = clock_div ;	// ʸ����
		nDetailTri = clock_point * 3 * 48 + 8 ;	// ���ݥꥴ��ߣ����ġܣͻ����͡ʣ��ݥꥴ���

		nHedronTri = (clock_hedron ? 20 : 8) * 12 ;			// Ȭ���Τ⤷���������Ρߣ�����
		nNeedleTri = /* 28 */ 42 * 3 ;		// �����ݥꥴ��ߣ���
	}
	else
	{
		nBoardTri = 0 ;	// ʸ����
		nDetailTri = 0 ;	// ���ݥꥴ��ߣ����ġܣͻ����͡ʣ��ݥꥴ���

		nHedronTri = 0 ;			// Ȭ���Τ⤷���������Ρߣ�����
		nNeedleTri = 0 ;		// �����ݥꥴ��ߣ���
	}

	// CULL_STATUS �ˤ�äƱƶ��������ñ�̤ǤΥݥꥴ���
//	int nCullNoneTri ;	// CULL_STATUS_SHELL �˳������ʤ��Ȥ��ʥǥե���ȡ�
//	int nCullShellTri ;	// CULL_STATUS_SHELL �˳�������Ȥ�����������
//	int nObjCullTri ;	// CULL_STATUS_FRONT �˳������ʤ��ե���ȥ��֥������ȤΥݥꥴ���
						// ����������ϥ���


#ifndef DRAW_DETAIL_FIRST_WALL
	// -r ���ץ����� -DETAIL ���ɲ�ʬ�ʳ̡ʤ������ǽ���ɤ�ʬ���ʤ��ˡ������ܡ��ɡ�
	int rDetail = (n_s_vtx[clock_mesh] - 3) * clock_div * 2	+ clock_div
				+ nRoofTri + nBoardTri ;

#else	// #ifndef DRAW_DETAIL_FIRST_WALL
	// -r ���ץ����� -DETAIL ���ɲ�ʬ�ʳ̡������ܡ��ɡ�
	int rDetail = (n_s_vtx[clock_mesh] - 2) * clock_div * 2	+ clock_div
				+ nRoofTri + nBoardTri ;
#endif	// #ifndef DRAW_DETAIL_FIRST_WALL ... #else


//	// -r ���ץ����� -DETAIL ���ɲ�ʬ�ʳ̡�CULL_STATUS_SHELL �ˤ�긺���ˡ�����
//	int rDetailCull = (n_s_vtx[clock_mesh] - 3) * clock_div * 2	//	+ clock_div
//					+ nRoofTri ;	// + nBoardTri ;


	// ����Ū�ʹ�����
	int base = nShellTri + nGoldenTri + nRoofTri + nBoardTri + nDetailTri + nHedronTri + nNeedleTri ;

	if (mode_normal || mode_env || mode_crystal || mode_solid == 1 || mode_solid == 2 || mode_solid >= 10)
	{
		// ����Ū�ʹ����λ��ס�normal, -e, -C, -s 1, 2(13)��
		nTriangles = base ;
	}
	else if (mode_solid == 3)
	{
		// ñ��ʣ��������󥰤λ��ס�-s 3��
		nTriangles = base * 2 ;
	}
	else	// if (mode_real)
	{
		// -r ���ץ����λ���
		// ������ɲå��ץ����ˤ���Ѳ�
		if (mode_real >= 10)
		{
			// �ޥ���ƥ���������Ѥˤ����ܿ���Ʊ��
			nTriangles = base ;

			if (reflection)
				nTriangles += nBoardTri		// ����ȿ�͡ʺ�ɸ������뤿��ޥ���ѥ�������
							+ 2 * clock_div	// ȿ�ͤ�����
							+ nDetailTri + nHedronTri + nNeedleTri ;
		}
		else
		{
			// -r 1, 2���ޥ���ƥ������������ǽ��

			// ���ܹ����ˡ��ޥ���ѥ��γ̡������ܡ��ɡʶ���ȿ�͡ˡ�ʸ���ץǥ��ƥ�������ɲ�
			nTriangles = base + nShellTri + nRoofTri + nBoardTri + nDetailTri ;

			if (reflection)
				nTriangles += + 2 * clock_div * 2	// ȿ�ͤ����ɡʻͳѥݥꥴ��ʣ���ˡߥ������ȿ��ߥޥ���ѥ���
							+ nDetailTri * 2		// �ǥ��ƥ�����ߥޥ���ѥ�
							+ nHedronTri + nNeedleTri ;	// ʸ���׾�ζ�°���֥������ȣ��󤺤�
		}

		// �ǥ��ƥ�����ʥޥ���ѥ��ˤξ��ϡ��̡������ܡ��ɤ�ʬ������ɲ�
		if (detail == 1 || detail == 2)
		{
			if (mode_real >= 10 && reflection)
				nTriangles += rDetail - nBoardTri ;	// -R ���ϥܡ��ɤ��ޥ���ѥ��Τ��ᡢ������Σ���򣱥ѥ��ˤޤȤ�뤳�Ȥ���ǽ
			else
				nTriangles += rDetail ;
		}
//		else if (detail == 3 && reflection)
//			nTriangles += nBoardTri ;	// -R �Σ��ѥ����ƥ���������ϡ��ܡ��ɤ����ϣ��ѥ�ɬ�סʶ���ȿ�ͤϺ�ɸ������뤿��ޥ���ѥ�������
	}

	// ����˥���󥰤��Ȥ߹�碌��� ...

	return nTriangles ;
}


// GL ��Ϣ���å�
void SetGL()
{
#ifdef IS_SUPPORTED_GLUT_FULL_SCREEN
	if (full)
	{
		glutFullScreen() ;
		logprintf("Enabe full screen.\n") ;
	}
#endif	// #ifdef IS_SUPPORTED_GLUT_FULL_SCREEN

	// ����⡼�ɼ��̻�
	SetClockRenderMode() ;

	// ���ܹ����ݥꥴ����ʥ��饹������
	nm_polygons = CalcClockTriangles() ;

	if (saver)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f) ;
	else
		glClearColor(currentBG[0], currentBG[1], currentBG[2], currentBG[3]) ;

	glShadeModel(GL_SMOOTH) ;
	glCullFace(GL_BACK) ;
	glEnable(GL_CULL_FACE) ;

	if (saver)
	{
		glEnable(GL_SCISSOR_TEST) ;
		logprintf("Enable scissor test.\n") ;
	}

	// Reset Matrix
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	gluLookAt(0.0f,0.0f,0.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f) ;

	glPolygonMode(GL_FRONT_AND_BACK, gl_pol_mode) ;
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, gl_pers_corr) ;
	glHint(GL_FOG_HINT, gl_fog_hint) ;

//	if (clock_glass == 1) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
//	else if (texture == 3 && solid) glBlendFunc(GL_ONE, GL_ONE) ;
//	else glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) ;
//	else glBlendFunc(GL_ONE, GL_ONE) ;

	glBlendFunc(GL_ONE, GL_ONE) ;

	if (dither) glEnable(GL_DITHER) ;
	else        glDisable(GL_DITHER) ;

	// Texture Object ����ѤǤ��ʤ����ϥƥ�����������ϤޤȤ�ƹԤ�
	if (!has_texture_object && texture) SetTexture() ;

	SetDepthBuffer() ;

	SetStaticLightModel() ;
	SetLightModel() ;
	SetFog() ;


//#ifdef GL_VERSION_1_1
	if (polygon_offset)
		glPolygonOffset(-zOffset, -1.0f) ;
	else if (has_polygon_offset && (envOpenGL == VOODOO_3DFX_ICD) && (env || reality || crystal))
		glPolygonOffset(0.0f, 0.0f) ;

	if (has_polygon_offset && (envOpenGL == VOODOO_3DFX_ICD) && (env || reality || crystal))
		glEnable(GL_POLYGON_OFFSET_FILL) ;
//#endif
}


void CaptureScreenShot()
{
	// �������ˤ�äƤ�̵��
	if (xsize <= 1 || ysize <= 1)
		return ;

	// �ɤ߹��ߥХåե��򡢸��ߤν񤭹��ߥХåե��˥��å�
	GLenum drawBuffer ;
	glGetIntegerv(GL_DRAW_BUFFER, (GLint *)(&drawBuffer)) ;
	glReadBuffer(drawBuffer) ;

	TRUEIMAGE *image ;
	image = (TRUEIMAGE *)malloc(sizeof(TRUEIMAGE)) ;
	if (!image) return ;
	image->dataptr = (unsigned char *)malloc(sizeof(unsigned char) * (xsize + 4) * ysize * 3) ;
	if (!image->dataptr) return ;

	image->width  = xsize ;
	image->height = ysize ;

	glReadPixels(0,0, image->width,image->height, GL_RGB, GL_UNSIGNED_BYTE, image->dataptr) ;

	// OpenGL �ϥ��������Τ���ȿž
	ReverseCoordinates(image, FALSE, TRUE) ;

	// �̤��ֹ�ǥե�����͡�����Ѳ�
	static int cerealNo = 0 ;
	String fileName ;
	fileName.Format("%s_%02d.ppm", FILE_NAME_SCREEN_SHOT, cerealNo) ;
	cerealNo ++ ;
	if (cerealNo > 99)
		cerealNo = 0 ;	// 0��99 �ޤ�

	FILE *scrout ;
	scrout = fopen(fileName, "wb") ;
	if (scrout)
	{
		outputppm(image, scrout, SIGNATURE_SCREEN_SHOT) ;
	}
	fclose(scrout) ;

	freeimage(image) ;
}


void CreateRoofGradation()
{
	int i, div = clock_div / 4 ;
	if (!div) div = 1 ;

	rgrad = new Vector3d[ngrad = div * 4] ;

	for (i = 0 ; i < 4 ; i ++)
	{
		int j ;
		for (j = 0 ; j < div ; j ++)
		{
			rgrad[i * div + j] = (rcol[i] * (div - j) + rcol[(i + 1) % 4] * j) / div ;
			if      (mode_real) rgrad[i * div + j] *= 0.7f ;
//			if      (reality == 1 || reality == 11 || reality == 21) rgrad[i * div + j] *= 0.5f ;
//			else if (reality == 2 || reality == 12 || reality == 22) rgrad[i * div + j] *= 0.7f ;
		}
	}


	float color_weight = 1.0f / default_anti ;
	rgrad_anti = new Vector3d[ngrad = div * 4] ;

	for (i = 0 ; i < 4 ; i ++)
	{
		int j ;
		for (j = 0 ; j < div ; j ++)
		{
			rgrad_anti[i * div + j] = (rcol[i] * (div - j) + rcol[(i + 1) % 4] * j) / div * color_weight ;
			if      (mode_real) rgrad_anti[i * div + j] *= 0.7f ;
//			if      (reality == 1 || reality == 11 || reality == 21) rgrad_anti[i * div + j] *= 0.5f ;
//			else if (reality == 2 || reality == 12 || reality == 22) rgrad_anti[i * div + j] *= 0.7f ;
		}
	}

	color_weight = 1.0f / default_field ;
	rgrad_field = new Vector3d[ngrad = div * 4] ;

	for (i = 0 ; i < 4 ; i ++)
	{
		int j ;
		for (j = 0 ; j < div ; j ++)
		{
			rgrad_field[i * div + j] = (rcol[i] * (div - j) + rcol[(i + 1) % 4] * j) / div * color_weight ;
			if      (mode_real) rgrad_field[i * div + j] *= 0.7f ;
//			if      (reality == 1 || reality == 11 || reality == 21) rgrad_field[i * div + j] *= 0.5f ;
//			else if (reality == 2 || reality == 12 || reality == 22) rgrad_field[i * div + j] *= 0.7f ;
		}
	}

	color_weight = 1.0f / default_motion ;
	rgrad_motion = new Vector3d[ngrad = div * 4] ;

	for (i = 0 ; i < 4 ; i ++)
	{
		int j ;
		for (j = 0 ; j < div ; j ++)
		{
			rgrad_motion[i * div + j] = (rcol[i] * (div - j) + rcol[(i + 1) % 4] * j) / div * color_weight ;
			if      (mode_real) rgrad_motion[i * div + j] *= 0.7f ;
//			if      (reality == 1 || reality == 11 || reality == 21) rgrad_motion[i * div + j] *= 0.5f ;
//			else if (reality == 2 || reality == 12 || reality == 22) rgrad_motion[i * div + j] *= 0.7f ;
		}
	}
}


void InitGlobalVal()
{
	title = "" ;
	tmpTitle = "" ;

	glut_gameMode = FALSE ;

	f_FPS = FORMAT_FPS ;
	f_FPS_kTPS = f_FPS + FORMAT_KTPS ;
	f_FPS_kTPS_kTPF = f_FPS_kTPS + FORMAT_KTPF ;

	// �٥���ޡ��������ѹ����٥ե����ޥå�
	d_FPS_kTPS_kTPF = String(FORMAT_LOG_FPS) + FORMAT_LOG_KTPS + FORMAT_LOG_KTPF ;

	firstError = TRUE ;

	keyL = OFF ;
	keyR = OFF ;
	keyU = OFF ;
	keyD = OFF ;

	firstFPS = TRUE ;
	startElapsedTime = 0 ;
	testCount = 0 ;
	benchmark = FALSE ;

	firstReshapeTime = -1 ;
	loop_count = 0 ;
	changeTitleWait = 0 ;
	sum_polygons = 0 ;
	sum_fps = 0.0f ;
	max_fps = 0.0f ;

	clockSize = 1.0f ;
	jitter = NULL ;
	jitter_dof = NULL ;
	fogFlag = FALSE ;

	solid_image = NULL ;
	env_image = NULL ;
	env2_image = NULL ;
	light_image = NULL ;
	solid2_image = NULL ;
	create_image = NULL ;
	specularImage = NULL ;
	multiTexSpecularImage = NULL ;

	metalic_image = NULL ;
	gold_image = NULL ;
//	gold2_image = NULL ;
	green_image = NULL ;
	blue_image = NULL ;
	red_image = NULL ;
	pink_image = NULL ;

	voodoo = FALSE ;
	mesa = FALSE ;
	fxOpenGL = FALSE ;
	drawCursor = FALSE ;
	polygon_offset = FALSE ;

	detail_texture_bright = 255.0f ;
	detail_texture_correct_bright = 1.0f ;


#ifdef WIN32
	cw = 0 ;
	ch = 0 ;
	hWin = NULL ;
	hIcon = NULL ;
	isChange_DisplaySettings = FALSE ;
#endif

	// �ƥ������㡦���֥������ȴ��������
	TextureObjectCtrl::textureObjectCtrl.Initialize() ;

	// �ƥ�������͡�������
	CHROME_TEXTURE_NAME = GOLD_TEXTURE_NAME = SPECULAR_TEXTURE_NAME = CHROME2_TEXTURE_NAME =
		GREEN_TEXTURE_NAME = BLUE_TEXTURE_NAME = RED_TEXTURE_NAME = PINK_TEXTURE_NAME =
			SOLID_TEXTURE_NAME = SOLID2_TEXTURE_NAME = DETAIL_TEXTURE_NAME =
				SPECULAR_MULTI_TEXTURE_NAME = (GLuint)0 ;

/*
	// �������֡�������������
	for (int l = 0 ; l < 8 ; l ++)
	{
		Vector3d u(ldir[l]) ;
		u.Unit() ;
		ldir[l][0] = u.X() ;
		ldir[l][1] = u.Y() ;
		ldir[l][2] = u.Z() ;

		sdir[l][0] = -u.X() ;
		sdir[l][1] = -u.Y() ;
		sdir[l][2] = -u.Z() ;

		lpos[l][0] = ldir[l][0] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][1] = ldir[l][1] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][2] = ldir[l][2] * POSITIONAL_LIGHT_DISTANCE ;
		lpos[l][3] = 1.0f ; // / POSITIONAL_LIGHT_W ;

		// master_lcol ���饳�ԡ�
//		for (int i = 0 ; i < 4 ; i ++)
//			lcol[l][i] = master_lcol[l][i] ;
	}
*/

	winIDglut = 0 ;
	firstReshapeTime = -1 ;

#ifdef WIN32
	g_hFileMap = NULL ;
	g_pFps = NULL ;
	hWin = NULL ;
	isExistWINNLSEnableIME = FALSE ;
	pWINNLSEnableIME = NULL ;
	hLibUser32 = NULL ;
#endif

#ifndef VOODOO_MODE
	WIDTH  = 320 ;
	HEIGHT = 320 ;
#else
	WIDTH  = 640 ;
	HEIGHT = 480 ;
#endif	// #ifndef VOODOO_MODE

	DONOT_USE_GL_READ_BUFFER = FALSE ;

	maxTextureUnits = 0 ;
	specularTimeGradRatio = 1.0f ;
	screen_size = 1.0f ;

	glVendor = "" ;
	glRenderer = "" ;
	glVersion = "" ;
	glExtensions = "" ;

	// ���ϥ��ȥ꡼��
	outstream = NULL ;

	// Quality
	for (int q = 0 ; q < 8 ; q ++)
	{
		for (int e = 0 ; e < 6 ; e ++)
			quality[q][e] = quality_master[q][e] ;
	}

	InitLightsAngles() ;

	currentBG[3] = 0.0f ;
}


GLint GetAvailableInternalFormat(GLint format, String *formatSymbol = NULL, String *formatInfos = NULL)
{
	if (GetOpenGLVersion() < 1.1f) // || mesa)
	{
		if (formatSymbol)
			*formatSymbol = InternalFormatString(format) ;

		if (formatInfos)
			*formatInfos = "" ;

		return format ;
	}

	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, format, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL) ;

	GLint internalFormat ;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat) ;

	if (formatSymbol)
		*formatSymbol = InternalFormatString(internalFormat) ;

	if (formatInfos && internalFormat)
	{
		GLint red = 0, green = 0, blue = 0, alpha = 0, luminance = 0, intensity = 0 ;

		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &red) ;
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, &green) ;
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, &blue) ;
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &alpha) ;
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_LUMINANCE_SIZE, &luminance) ;
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_INTENSITY_SIZE, &intensity) ;

		*formatInfos = "" ;

		// ���餫���������ͤ�̵��
		if (red			> 0 && red       <= 256)	*formatInfos += "R:" + IntToString(red  ) + " " ;
		if (green		> 0 && green     <= 256)	*formatInfos += "G:" + IntToString(green) + " " ;
		if (blue		> 0 && blue      <= 256)	*formatInfos += "B:" + IntToString(blue ) + " " ;
		if (luminance	> 0 && luminance <= 256)	*formatInfos += "Luminance:" + IntToString(luminance) + " " ;
		if (alpha		> 0 && alpha     <= 256)	*formatInfos += "A:" + IntToString(alpha) + " " ;
		if (intensity	> 0 && intensity <= 256)	*formatInfos += "Intensity:" + IntToString(intensity) + " " ;

		formatInfos->Cut() ;
	}
	else if (formatInfos)
		*formatInfos = "" ;

	return internalFormat ;
}


// �����
void Initialize(Option& option)
{
	InitGlobalVal() ;
	InitVar(option) ;

	StartupMessage() ;
	glClientInformationsLog = MakeOpenGLClientInformationsLog() ;
	logprintf(glClientInformationsLog) ;

	InitWindow(option) ;

	{
		const char *glStr ;
		glStr = (const char *)glGetString(GL_VENDOR) ;
		if (glStr) glVendor = glStr ;
		glStr = (const char *)glGetString(GL_RENDERER) ;
		if (glStr) glRenderer = glStr ;
		glStr = (const char *)glGetString(GL_VERSION) ;
		if (glStr) glVersion = glStr ;

		OpenGL_Version = GetOpenGLVersion() ;

		glStr = (const char *)glGetString(GL_EXTENSIONS) ;
		if (glStr)
		{
			char *str = new char[strlen(glStr) + 1] ;
			char *token ;
			int  recLen = 0, count = 0 ;

			strcpy(str, glStr) ;
			token = strtok(str, " ") ;

			while(token)
			{
//				String sp = ("  ") ;
				String ext = token ;
				recLen += ext.Length() + 4 ;
				count ++ ;

				if (recLen >= 56 || count >= 4)
				{
					glExtensions += "\n" ;
					count = 0 ;
					recLen = 8 ;
				}

				glExtensions += "\t" + ext ;
				token = strtok(NULL, " ") ;
			}

			delete [] str ;
		}
	}

	// OpenGL �¹ԴĶ��ˤ��ػߵ�ǽ�ʤ�
	AdjustRunTimeOpenGLEnvironmentFromRenderer() ;


	// OpenGL Extension ��Ϣ
	SetupOpenGLExtensions() ;

	tex_internal_format = InternalFormat(tex_format) ;
	tex_internal_format = GetAvailableInternalFormat(tex_internal_format, &internalFormatSymbol, &internalFormatInfos) ;

	// �ƥ�������κ��祵����
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize) ;

	glServerInformationsLog = MakeOpenGLServerInformationsLog() ;
	colorBufferLog = MakeColorBufferInfoLog() ;
	glInformationsLog = glClientInformationsLog + glServerInformationsLog ;

	// OpenGL Info �ˤϥ��åȤ��줿������ɥ����������ɲ�
	glInformationsLog += "\n" + colorBufferLog ;

	logprintf(glServerInformationsLog + "\n") ;
	logprintf(colorBufferLog) ;

	// -foutgl ���ץ����⤷���� -tres 0 ���ϡ�Elapsed Time ��ͭ���ʴֳ֤�¬��
	if (fout_gl != "" || timerResolution == 0)
	{
		timerResolution = (unsigned int)(TimerResolution() + 0.5f) ;
		glInformationsLog += "Timer resolution (m-sec): " + IntToString((int)timerResolution) + "\n" ;
	}

/*
	if (OpenGL_Version >= 1.1f && zOffset > 0.0f)
		polygon_offset = TRUE ;
	else
	{
		zOffset = 0.0f ;
		polygon_offset = FALSE ;
	}
*/

	if (fout_gl == "")
	{
		if (glut_buff == GLUT_SINGLE) logprintf("Single buffering mode.\n") ;
		else                          logprintf("Double buffering mode.\n") ;
	}

//	if (title == TITLE_VER && !saver)
	if (!saver)
	{
//		title += " -- " + glVendor + " - " + glRenderer + " - " + glVersion + " --" ;
		title += " -- " + glRenderer + " - " + glVersion + " - " + glVendor + " --" ;
		SetWindowTitle(title) ;
	}

	// -foutgl ���ץ����ǥե����뤬���ꤵ�줿���ϡ������˥ե������������ƾ�������
	if (fout_gl != "")
	{
		FILE *fp ;

		if (!(fp = fopen(fout_gl, "w")))
		{
#ifdef WIN32
			MessageBox(NULL, "Can't open output file: " + fout_gl, "File Error", MB_OK | MB_ICONSTOP) ;
#endif
			errprintf(String("Can't open output file: \"") + fout_gl + "\"\n") ;
			EndProc() ;
			glclock_exit(EXIT_FAILURE) ;
		}

		fprintf(fp, TITLE_VER) ;
		if (benchmark)
			fprintf(fp, String("\n") + AUTHOR + "\n\n") ;
//			fprintf(fp, String(" Benchmark Test.\n") + AUTHOR + "\n\n") ;
		else
			fprintf(fp, String("\n") + AUTHOR + "\n\n") ;

		fprintf(fp, glInformationsLog + "\n") ;
		fclose(fp) ;

		return ;
	}

/*
	// ��������ɽ���� On/Off
	// voodoo �⡼�ɡ��Ĥޤ� 3Dfx OpenGL Beta-2.1 �ޤ��� MesaGlide �ޤ��ϡ�
	// 3Dfx OpenGL Client Driver �ǡ������餬 Voodoo3 �Ǥ� Banshee �Ǥ� Rush �Ǥ�ʤ����
	if (voodoo ||
		(envOpenGL == VOODOO_3DFX_ICD &&
		 !glRenderer.Search("Voodoo3") && !glRenderer.Search("Banshee") && !glRenderer.Search("Rush"))
		)
	{
		voodoo = TRUE ;
		drawCursor = TRUE ;

//#if defined WIN32
//		if ((!(cw == 512 && ch == 384) &&
//			 !(cw == 640 && ch == 480) &&
//			 !(cw == 1024 && ch == 768))
//			 || !full)
//		{
//			full = TRUE ;
//			is_set_width  = xsize = 640 ;
//			is_set_height = ysize = 480 ;
//			SetDisplayMode(640, 480, cw, ch) ;
//			glutFullScreen() ;
//		}
//#endif
	}
*/

	// -LIGHTTWOSIDE �ζ��������3Dfx Beta 2.1 �ξ�粿�Τ���������®�������
	// -VOODOO ���ץ���󤬻��ꤵ��Ƥ��ʤ����⡢��󥿥���ΥС��������󤫤�Ƚ�Ǥ���
	// ��ưŪ�˥��å�
//	if (glVersion.Search("3Dfx Beta 2.1"))
//		lightTwoSide = TRUE ;

	SetGL() ;

	InitRotate() ;
	CreateRoofGradation() ;

	if (anti) jitter = CreateSampleJitter(anti) ;
	if (!jitter) anti = 0 ;
	if (field) jitter_dof = CreateSampleDepthOfFieldJitter(field) ;
	if (!jitter_dof) field = 0 ;


	// �٥���ޡ����Ѥ˥����ޡ������ټ����ʥǥե���Ȥξ��Τߡ�
	// Windows �ǥǥե���Ȥξ��ΤߣϣӤ������
	// Windows �ʳ��Ǥϡ�-tres �Υǥե�����ͤ����åȤ��줿�ޤޤˤʤ�
#ifdef WIN32
	if (timerResolution == TIME_RESOLUTION)
	{
		TIMECAPS timeCaps ;
		timeGetDevCaps(&timeCaps, sizeof(TIMECAPS)) ;
		timerResolution = timeCaps.wPeriodMin ;
	}
#endif

	// �٥���ޡ����κݤβ�ž®�ٴ���360.0f��1440.0f�� --> ��120.0f��960.0f��
	master_spin_ratio = 3600.0f / timerResolution ;	// 7200.0f --> 3600.0f
	if (master_spin_ratio > 960.0f)
		master_spin_ratio = 960.0f ;
	else if (master_spin_ratio < 90.0f)
		master_spin_ratio = 90.0f ;

// Win32 Mesa �Ǥϥꥹ�ȥ�����������ǽ��
//#if defined __WIN32__ && defined MESA
//	cerealListNo = 0 ;
//#endif	// #if defined __WIN32__ && defined MESA
}

// �����ܡ��ɥ��٥�ȥϥ�ɥ�
void HandleKey(unsigned char key, int x, int y)
{
	// �٥���ޡ������� ESC �������ϰʳ��ϼ����դ��ʤ�
    if (benchmark)
	{
		if (key == 0x1b)
		{
			EndProc() ;
			glclock_exit(EXIT_SUCCESS) ;
		}
		else return ;
	}

    switch (key)
	{
	case 0x1b:	// ESC �ǽ�λ
		EndProc() ;
		glclock_exit(EXIT_SUCCESS) ;

	case ' ':	// ���ڡ����ǳ�����
		t_roof = 1.0f - t_roof ;
		break ;

	case 's':	// 's' �ޤ��� 'S' �����
	case 'S':
		spin_x *= 0.1f ;
		spin_y *= 0.1f ;
		break ;

	case 'a':	// 'a' �ޤ��� 'A' �ǥ���ƥ������ꥢ����
	case 'A':
		if (!anti)
		{
			anti = default_anti ;
			if (!jitter) jitter = CreateSampleJitter(anti) ;

			// VOODOO �Ǥ�ξ��������Ȥ�Ф�
//			if (voodoo) motion = OFF ;

			// ��̳����٥֥顼��¾�Υ����ѥ���ץ�󥰤���¾
			field = OFF ;

			// multi_super_sample �� FALSE �ξ���Ʊ��������ػ�
			if (!multi_super_sample)
				motion = OFF ;

			if (flag_additiveAlphaSuperSampling)
				SetTemporaryWindowTitle("Enable " + IntToString(anti) + " sampled scene anti-aliasing (with additive alpha blending)") ;
			else if (flag_withoutAccumBufferSuperSampling)
				SetTemporaryWindowTitle("Enable " + IntToString(anti) + " sampled scene anti-aliasing (with alpha blending)") ;
			else
				SetTemporaryWindowTitle("Enable " + IntToString(anti) + " sampled scene anti-aliasing (with accum-buffer)") ;
		}
		else
		{
			anti = OFF ;
			SetTemporaryWindowTitle("Disable scene anti-aliasing") ;
		}
		break ;

	case 'd':	// 'd' �ޤ��� 'D' �ǥǥץ����֥ե�����ɥ֥顼
	case 'D':
		if (!field)
		{
			field = default_field ;
			if (!jitter_dof) jitter_dof = CreateSampleDepthOfFieldJitter(field) ;

			// ��̳����٥֥顼��¾�Υ����ѥ���ץ�󥰤���¾
			anti = OFF ;
			motion = OFF ;

			if (flag_additiveAlphaSuperSampling)
				SetTemporaryWindowTitle("Enable " + IntToString(field) + " sampled depth of field blur (with additive alpha blending)") ;
			else if (flag_withoutAccumBufferSuperSampling)
				SetTemporaryWindowTitle("Enable " + IntToString(field) + " sampled depth of field blur (with alpha blending)") ;
			else
				SetTemporaryWindowTitle("Enable " + IntToString(field) + " sampled depth of field blur (with accum-buffer)") ;
		}
		else
		{
			field = OFF ;
			SetTemporaryWindowTitle("Disable depth of field blur") ;
		}
		break ;

	case 'm':	// 'm' �ޤ��� 'M' �ǥ⡼�����֥顼
	case 'M':
		pre_motion = motion ;		// ľ���� motion �ͤ�Ͽ
		if (!motion)
		{
			motion = default_motion ;

			// VOODOO �Ǥ�ξ��������Ȥ�Ф���
//			if (voodoo) anti = OFF ;

			// ��̳����٥֥顼��¾�Υ����ѥ���ץ�󥰤���¾
			field = OFF ;

			// multi_super_sample �� FALSE �ξ���Ʊ��������ػ�
			if (!multi_super_sample)
				anti = OFF ;

			if (flag_additiveAlphaSuperSampling)
			{
				SetTemporaryWindowTitle("Enable " + IntToString(motion) + " sampled motion-blur (with additive alpha blending)") ;
			}
			else if (flag_withoutAccumBufferSuperSampling)
			{
				SetTemporaryWindowTitle("Enable " + IntToString(motion) + " sampled motion-blur (with alpha blending)") ;
			}
			else
				SetTemporaryWindowTitle("Enable " + IntToString(motion) + " sampled motion-blur (with accum-buffer)") ;
		}
		else
		{
			motion = OFF ;
			SetTemporaryWindowTitle("Disable motion-blur") ;
		}
		break ;

	case 'n':	// 'n' �ޤ��� 'N' ���ÿˤ����⡼�����֥顼
	case 'N':
		if (!s_motion)
		{
			s_motion = pre_s_motion ;
			SetTemporaryWindowTitle("Enable " + IntToString(s_motion) + " sampled second hand motion-blur (with alpha blending)") ;
		}
		else
		{
			s_motion = OFF ;
			SetTemporaryWindowTitle("Disable second hand motion-blur") ;
		}
		break ;

	case 'q':	// 'q' �ޤ��� 'Q' �ǥե����ʤ�
	case 'Q':
		fogFlag = FOG_NONE ;
		SetFog() ;
		break ;

	case 'w':	// 'w' �ޤ��� 'W' �ǥ�˥��⡼�ɤΥե���
	case 'W':
		fogFlag = FOG_LINEAR ;
		SetFog() ;
		break ;

	case 'e':	// 'e' �ޤ��� 'E' �� exp �⡼�ɤΥե���
	case 'E':
		fogFlag = FOG_EXP ;
		SetFog() ;
		break ;

	case 'r':	// 'r' �ޤ��� 'R' �� exp2 �⡼�ɤΥե���
	case 'R':
		fogFlag = FOG_EXP2 ;
		SetFog() ;
		break ;

	case 't':	// 't' �ޤ��� 'T' �� �ƥ�������ե��륿�ѹ�
	case 'T':
		if (mode_normal)
		{
			SetTemporaryWindowTitle("Ignoring ... non-texturing mode") ;
			break ;
		}

		if (textureFilter.Character(1) == 'N')
			textureFilter.SetAt(1, 'L') ;
		else
			textureFilter.SetAt(1, 'N') ;

		UpdateTextureFilter() ;
		logprintf(TextureFilteringLog() + ".\n") ;
		break ;

	case 'y':	// 'y' �ޤ��� 'Y' �� �ߥåץޥåץե��륿�ѹ�
	case 'Y':
		if (mode_normal)
		{
			SetTemporaryWindowTitle("Ignoring ... non-texturing mode") ;
			break ;
		}

		if (textureFilter.Character(2) == '\0')
			textureFilter += "N" ;
		else if (textureFilter.Character(2) == 'N')
			textureFilter.SetAt(2, 'L') ;
		else
			textureFilter = textureFilter.Character(1) ;

		UpdateTextureFilter() ;
		logprintf(TextureFilteringLog() + ".\n") ;
		break ;

	case 'u':	// 'u' �ޤ��� 'U' �ǲ�ǽ�ʤ�� Anisotropic Filtering ON/OFF
	case 'U':
		if (!texture)
		{
			SetTemporaryWindowTitle("Ignoring ... non-texturing mode") ;
			break ;
		}
		else if (default_maxAnisotropy <= 1.0f)
		{
			SetTemporaryWindowTitle("Anisotropic filtering is not supported") ;
			break ;
		}

		if (maxAnisotropy <= 1.0)
		{
			// ON
			maxAnisotropy = default_maxAnisotropy ;
			UpdateAnisotropy(maxAnisotropy) ;

			String log = "Enable level " + IntToString((int)maxAnisotropy) + " anisotropic texture filtering (" + texture_filter_anisotropic_name + ")" ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		else
		{
			// OFF
			maxAnisotropy = 1.0f ;
			UpdateAnisotropy(maxAnisotropy) ;

			String log = "Disable anisotropic texture filtering" ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		break ;

	case 'f':	// 'f' �ޤ��� 'F' �ǣƣУ�ɽ��
	case 'F':
		if (!f_prn)
		{
			f_prn = ON ;
			logprintf("\n") ;
		}
		else        f_prn = OFF ;
		break ;

	case 'l':	// 'l' �ޤ��� 'L' �ǥ�������ӥ塼���ưŪ ON/OFF
	case 'L':	// �����������ࡼ���������ǥ����������Τ��� -LO ���ץ����Ȥޤä���Ʊ���ˤϤʤ�ʤ�
		if (local)
			local = GL_FALSE ;
		else
			local = GL_TRUE ;

		SetLightModel() ;
		break ;

	case 'I':	// 'I' �ޤ��� 'i' ��ʿ�Ը���
	case 'i':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}
//		if (lightType == LIGHT_TYPE_DIRECTIONAL) break ;
		lightType = LIGHT_TYPE_DIRECTIONAL ;
		SetLightModel() ;
		break ;

	case 'O':	// 'O' �ޤ��� 'o' ��������
	case 'o':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}
//		if (lightType == LIGHT_TYPE_POINT) break ;
		lightType = LIGHT_TYPE_POINT ;
		SetLightModel() ;
		break ;

	case 'P':	// 'P' �ޤ��� 'p' �ǥ��ݥåȥ饤��
	case 'p':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}
//		if (lightType == LIGHT_TYPE_SPOT) break ;
		lightType = LIGHT_TYPE_SPOT ;
		SetLightModel() ;
		break ;

//	case '0':	// 1 �� 8 �ǡ��饤�ȿ���ưŪ���ѹ�
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':

	case '6':
	case '7':
	case '8':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}
		else
		{

			int l = int(key - '0') ;
			if (l == light) break ;
			light = l ;
			SetLightModel() ;
		}
		break ;

	case 'K':	// 'K' �ޤ��� 'k' �� LightTest�ʥ饤�Ȳ�ž�˥ե饰�ڤ��ؤ�
	case 'k':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}

		rotateLight = !rotateLight ;
		InitLightsAngles() ;
		SetLightModel(FALSE) ;

		{
			String log ;
			if (rotateLight)
				log = "Enable rotating light" ;
			else
				log = "Disable rotating light" ;

			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		break ;

	case 'h':	// 'h' �ǥ饤�Ȥκ��ٸ���
//	case 'H':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}

		light_vividness -= 0.2f ;
		logprintf(SetLightColorTableFromSaturation() + ".\n") ;
		SetLightModel(FALSE) ;
		break ;

	case 'j':	// 'j' �ǥ饤�Ȥκ�������
//	case 'J':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}

		light_vividness += 0.2f ;
		logprintf(SetLightColorTableFromSaturation() + ".\n") ;
		SetLightModel(FALSE) ;
		break ;

	case 'H':	// 'H' �ޤ��� 'J' �ǥ饤�Ȥκ��٥ǥե����
	case 'J':
		if (!vertexShadingFlag)
		{
			SetTemporaryWindowTitle("Ignoring ... non-lighting mode") ;
			break ;
		}

		light_vividness = 1.0f ;
		logprintf(SetLightColorTableFromSaturation() + ".\n") ;
		SetLightModel(FALSE) ;
		break ;

	case 'z':	// 'z' �ǥǥץ����֥ե�����ɥ֥顼�Τ֤츺��
		{
			String log ;
			dof_range -= 0.2f ;
			log.Format("Depth of field blur parameter: %3.1f", dof_range) ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		break ;

	case 'x':	// 'x' �ǥǥץ����֥ե�����ɥ֥顼�Τ֤�����
		{
			String log ;
			dof_range += 0.2f ;
			log.Format("Depth of field blur parameter: %3.1f", dof_range) ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		break ;

	case 'Z':	// 'Z' �ޤ��� 'Z' �ǥǥץ����֥ե�����ɥ֥顼�Τ֤�ǥե����
	case 'X':
		{
			String log ;
			dof_range = 1.0f ;
			log.Format("Depth of field blur parameter: %3.1f", dof_range) ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
		}
		break ;

	case 'c':	// 'c' �ޤ��� 'C' �ǥ����꡼�󥭥�ץ���
	case 'C':
		{
			String log ;
			log = "Capturing screen shot" ;
			SetTemporaryWindowTitle(log) ;
			logprintf(log + ".\n") ;
			CaptureScreenShot() ;
		}
		break ;


#ifdef GIFF_DEMO
	case '0':
		Redisplay() ;
#endif

//	default:	// �����꡼�󥻡��Фξ�硢�������ʤ������ʤ齪λ
//		ExitSaver() ;
//		break ;
	}
}


// glut ������Хå���Ͽ
void SetCallback()
{
	glutDisplayFunc(DrawModel) ;
	glutReshapeFunc(ReshapeWindow) ;

	if (saver)
	{
		glutKeyboardFunc(ExitSaver) ;
//		glutKeyboardFunc(HandleKey) ;	// �����꡼�󥻡��ФǤ�����ǽ��	

		glutMouseFunc(ExitSaver) ;
		glutSpecialFunc(ExitSaver) ;
		glutMotionFunc(ExitSaver) ;

		glutPassiveMotionFunc(ExitSaverWait) ;

		glutIdleFunc(Redisplay) ;
	}
	else
	{
		glutKeyboardFunc(HandleKey) ;

#ifndef GIFF_DEMO
		if (!benchmark)
		{
			glutMouseFunc(MouseProc) ;
			glutSpecialFunc(SpecialKeyProc) ;

#ifdef IS_SUPPORTED_GLUT_KEYBOARD_STATUS
			glutSpecialUpFunc(SpecialKeyUpProc) ;
#endif

			glutMotionFunc(MouseMotion) ;
		}
		glutIdleFunc(Redisplay) ;
#endif
	}
}

void SignalIntProcedure(int sig)
{
	EndProc() ;
	glclock_exit(EXIT_FAILURE) ;
}


// for C interface
// and Windows DLL export
#ifdef WIN32
DLL_EXPORT BOOL CCONV DllMain(HANDLE hInst,
							  ULONG ul_reason_for_call,
							  LPVOID lpReserved)
{
	return TRUE ;
}
#endif


// for C interface
DLL_EXPORT int CCONV glclock_arg(int argc, char** argv)
{
#ifdef WIN32
	// �����꡼�󥻡��Х⡼�ɤǵ�ư��Τ�Τ������
	// ���ξ�ǽ�λ
	HWND win ;
	win = FindWindow("GLUT", "glclock screen saver") ;
	if (win) return EXIT_FAILURE ;

#endif

	Option option(argc, argv) ;
//	signal(SIGINT, SignalIntProcedure) ;

#ifdef GLCLOCK_RETURN_LONGJMP
	int ret = setjmp(glclock_env) ;
	if (ret)
	{
		// ��λ�����ɥ����å�
		if (ret == GLCLOCK_RET_LONGJMP_SUCCESS)
			return EXIT_SUCCESS ;
		else
			return EXIT_FAILURE ;
	}
#endif

	Initialize(option) ;

	if (fout_gl != "")
		return EXIT_SUCCESS ;

	MakeDisplayLists() ;

	SetCallback() ;
	start = TRUE ;

#ifdef macintosh
	InitCursor() ;	// �������������᤹
#endif

	DrawModel() ;	// ���ơ�������¸�Τ��ᡢ���٥�����

	logprintf("\n") ;


//		MessageBox(NULL, String("texture: ") + IntToString(texture) + "  solid: " + IntToString(solid), _T(""), MB_OK | MB_ICONSTOP) ;

	glutMainLoop() ;
	return EXIT_SUCCESS ;
}


// for C interface
DLL_EXPORT int CCONV glclock_opt(char *opt)
{
	// ���ץ�����ʸ����ơ��֥��ʬΥ
	String option(opt) ;
	option.Cut() ;
	int argc = 1 ;
	char **argv = (char **)malloc(sizeof(char *) * (argc + 1)) ;
//	argv[0] = "glclock" ;
	argv[0] = const_cast<char*>("glclock");

	if (option != "")	// ���餫�Υ��ץ���󤬻��ꤵ��Ƥ�����
	{
		while (TRUE)
		{
			int start = 1 ;
			int dFlag = FALSE ;
			int del ;

			// �ĤäƤ��� option �κ�ü����������
			while (start <= option.Length() && option.Character(start) == ' ') option.Right(option.Length() - 1) ;

			if (start > option.Length())
				break ;

			if (option.Character(start) == '\'' || option.Character(start) == '\"')
			{
				// ' �ޤ��� " �ξ�硢����˰Ϥޤ줿ʸ�������ڤ�Ȥ���
				dFlag = TRUE ;
				option.Right(option.Length() - 1) ;
			}

			if (dFlag)
			{
				// ' �ޤ��� " �ǻϤޤäƤ�����ϡ��б�����ʸ���ޤǤ���ڤ�Ȥ���
				del = start ;
				while (del <= option.Length() && option.Character(del) != '\'' && option.Character(del) != '\"')
					del ++ ;

				// del = ��λ�����ΰ���
//				if (del <= option.Length())
				del -- ;
			}
			else
			{
				del = option.Search(' ') ;
				if (del)
					del -- ;	// ����ΰ��� - 1
				else
					del = option.Length() ;
			}

			// start ʸ���ܤ��顢del ʸ���ܤޤǤ������ڤ�
			String token = Mid(option, start, del - start + 1) ;

			argc ++ ;
			argv = (char **)realloc(argv, sizeof(char *) * (argc + 1)) ;
			argv[argc - 1] = new char[token.Length() + 1] ;
			strcpy(argv[argc - 1], token) ;

			// ���Υ������Ȱ��֡��̾���ڤ�ζ���μ��ΰ��֡�
			if (dFlag) del ++ ;

			if (del >= option.Length() - 1)
				break ;

			option.Right(option.Length() - del) ;
		}
	}


	// �����ơ��֥�κǸ�� NULL
	argv[argc] = NULL ;

	// glclock ��ư
	int ret ;
	ret = glclock_arg(argc, argv) ;

	for (int i = 0 ; i < argc ; i ++)
		delete [] argv[i] ;

	free(argv) ;

	return ret ;

/*
	char *str = new char[strlen(opt)] ;
	char *token ;
	memcpy(str, opt, strlen(opt)) ;

	int argc = 1 ;
	char **argv = (char **)malloc(sizeof(char **) * (argc + 1)) ;
	argv[0] = "glclock" ;

	token = strtok(str, " ") ;
	while(token)
	{
		argc ++ ;
		argv = (char **)realloc(argv, sizeof(char **) * (argc + 1)) ;

		argv[argc - 1] = new char[strlen(token)] ;
		memcpy(argv[argc - 1], token, strlen(token)) ;

		token = strtok(NULL, " ") ;
	}

	// �����ơ��֥�κǸ�� NULL
	argv[argc] = NULL ;
	delete [] str ;

	int ret ;
	ret = glclock_arg(argc, argv) ;

	for (int i = 0 ; i < argc ; i ++)
		delete argv[i] ;

	free(argv) ;

	return ret ;
*/
}


// for C++ interface
int glclock(int argc, char** argv)
{
	return glclock_arg(argc, argv) ;
}

int glclock(char *opt)
{
	return glclock_opt(opt) ;
}
