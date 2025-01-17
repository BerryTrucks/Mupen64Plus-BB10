/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/keycodes.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
//#include <plugin/plugin.h>

#include <bps/dialog.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <dirent.h>

#define USING_GL20

#include "bbutil.h"

#ifdef USING_GL11
#include <GLES/gl.h>
#include <GLES/glext.h>
#elif defined(USING_GL20)
#include <GLES2/gl2.h>
#else
#error bbutil must be compiled with either USING_GL11 or USING_GL20 flags
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "png.h"

EGLDisplay egl_disp = 0;
EGLSurface egl_surf = 0;
font_t* font;

EGLSurface egl_surf_overlay = 0;
screen_window_t screen_win_overlay = 0;

static EGLConfig egl_conf;
static EGLContext egl_ctx;

screen_context_t screen_cxt;
screen_window_t screen_win;
screen_window_t back_screen = 0;
static screen_display_t screen_disp;
static int nbuffers = 2;
static int initialized = 0;

#ifdef USING_GL20
GLuint text_rendering_program;
int text_program_initialized = 0;
GLint positionLoc;
GLint texcoordLoc;
GLint textureLoc;
GLint colorLoc;
#endif

int disableSound,videoPlugin;
char romName[256] = {0};
int initialized_n64 = 0;
UIQuad* overlayKey = NULL;
UIQuad* overlayQuad = NULL;
UIQuad* stickQuad = NULL;
UIQuad* stickQuad2 = NULL;
UIQuad* osd_save = NULL;
UIQuad* osd_load = NULL;
float save = 0.0f, load = 0.0f;
int overlay_request = 0;

double DISPLAY_WIDTH = 1280.0;
double DISPLAY_HEIGHT = 768.0;

int hdmi_width = -1;
int hdmi_height = -1;
bool use_hdmi = false;
bool use_overlay = false;
bool dbg_fps = false;
int q10_rotate = 0;
bool use_gamepad = false;
bool egl_capture = false;
int capture_count = -1;
bool m64p_emit_touch = false;

m64p_touch_overlay_callback touch_overlay_callback = 0;
m64p_capture_callback capture_callback = 0;
m64p_touch_callback touch_callback = 0;

uint margin_bottom = 0;
uint margin_left = 0;

static void
bbutil_egl_perror(const char *msg) {
    static const char *errmsg[] = {
        "function succeeded",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "cannot access a requested resource",
        "failed to allocate resources for the requested operation",
        "an unrecognized attribute or attribute value was passed in an attribute list",
        "an EGLConfig argument does not name a valid EGLConfig",
        "an EGLContext argument does not name a valid EGLContext",
        "the current surface of the calling thread is no longer valid",
        "an EGLDisplay argument does not name a valid EGLDisplay",
        "arguments are inconsistent",
        "an EGLNativePixmapType argument does not refer to a valid native pixmap",
        "an EGLNativeWindowType argument does not refer to a valid native window",
        "one or more argument values are invalid",
        "an EGLSurface argument does not name a valid surface configured for rendering",
        "a power management event has occurred",
        "unknown error code"
    };

    int message_index = eglGetError() - EGL_SUCCESS;

    if (message_index < 0 || message_index > 14)
        message_index = 15;

    fprintf(stderr, "%s: %s\n", msg, errmsg[message_index]);
}

void begin_capture()
{
    egl_capture = true;
}

bool egl_letterbox = false;
bool egl_pillarbox = false;
int egl_bottom, egl_left, egl_width, egl_height;

void PB_eglSwapBuffers() {
    if (egl_letterbox)
    {
        glEnable(GL_SCISSOR_TEST);
        float scissor[4];
        glGetFloatv(GL_SCISSOR_BOX, scissor);
        glClearColor(0, 0, 0, 1.0);
        glScissor(0, 0, egl_width, egl_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glScissor(0, egl_bottom, egl_width, egl_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    }
    else if (egl_pillarbox)
    {
        glEnable(GL_SCISSOR_TEST);
        float scissor[4];
        glGetFloatv(GL_SCISSOR_BOX, scissor);
        glClearColor(0, 0, 0, 1.0);
        glScissor(0, 0, egl_width, egl_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glScissor(egl_left, 0, egl_width, egl_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    }
    /*if (margin_left || margin_bottom)
    {
        glEnable(GL_SCISSOR_TEST);
        float scissor[4];
        glGetFloatv(GL_SCISSOR_BOX, scissor);
        glClearColor(0, 0, 0, 1.0);
        //if (margin_bottom)
        //    glScissor(0, 0, DISPLAY_WIDTH, margin_bottom);
        //else
        //    glScissor(0, 0, margin_left, DISPLAY_HEIGHT);
        //glClear(GL_COLOR_BUFFER_BIT);
        if (margin_bottom)
            glScissor(0, DISPLAY_HEIGHT - (margin_bottom << 1), DISPLAY_WIDTH, margin_bottom);
        else
            glScissor(DISPLAY_WIDTH - (margin_left << 1), 0, margin_left, DISPLAY_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);
        glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    }*/
    if (use_overlay) {
        eglSwapBuffers(egl_disp, egl_surf);
        bool currentScissor = glIsEnabled(GL_SCISSOR_TEST);
        glDisable(GL_SCISSOR_TEST);
        eglMakeCurrent(egl_disp, egl_surf_overlay, egl_surf_overlay, egl_ctx);
        glClearColor(0, 0, 0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        if (currentScissor)
            glEnable(GL_SCISSOR_TEST);
        if (touch_overlay_callback)
            (*touch_overlay_callback)();
        eglSwapBuffers(egl_disp, egl_surf_overlay);
        eglMakeCurrent(egl_disp, egl_surf, egl_surf, egl_ctx);
    }
    else
    {
        if (egl_capture)
        {
            if (capture_count == -1)
                capture_count = 200;
            else if (capture_count == 0)
                egl_capture = false;
            if (capture_count % 10 == 0)
            {
                eglSwapBuffers(egl_disp, egl_surf);
                int size = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;
                unsigned char *pucFrame = (unsigned char *) malloc(size);
                glReadPixels(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pucFrame);
                (*capture_callback)(capture_count, pucFrame, size);
                free(pucFrame);
            }
            else
            {
                if (touch_overlay_callback)
                    (*touch_overlay_callback)();
                eglSwapBuffers(egl_disp, egl_surf);
            }
            capture_count--;
        }
        else
        {
            if (touch_overlay_callback)
                (*touch_overlay_callback)();
            eglSwapBuffers(egl_disp, egl_surf);
        }
    }
}

/**
 * Use the PID to set the window group id.
 */
char *
get_window_group_id()
{
    static char s_window_group_id[16] = "";

    if (s_window_group_id[0] == '\0') {
        snprintf(s_window_group_id, sizeof(s_window_group_id), "%d", getpid());
    }

    return s_window_group_id;
}

void bbutil_defocus()
{
	int z = 2;
	screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &z);
	if (back_screen)
	{
	    z = 1;
	    screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_ZORDER, &z);
	}
}

static int menuOpen = 0;

int bbutil_offset_menu(int offset)
{
	int pos[2];// = { margin_left, offset + margin_bottom };
	if (q10_rotate != 0)
	{
	    pos[0] = q10_rotate * -offset;
	    pos[1] = 0;
	}
	else
	{
        pos[0] = margin_left;
        pos[1] = offset + margin_bottom;
	}
    int pos2[2] = { 0, offset };
    if (!use_hdmi)
    {
        screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);
        screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_POSITION, pos2);
    }
    screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_POSITION, pos2);
	if (offset > 0)
	{
		menuOpen = 1;
		m64p_emit_touch = 1;
		bbutil_defocus();
	}
	else
	{
		menuOpen = 0;
		m64p_emit_touch = 0;
		bbutil_focus();
	}
	return 0;
}

int bbutil_close_menu()
{
	int pos[2] = { margin_left, margin_bottom };
    int pos2[2] = { 0, 0 };
    if (!use_hdmi)
    {
        screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);
        screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_POSITION, pos2);
    }
    screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_POSITION, pos2);
	menuOpen = 0;
	bbutil_focus();
	return 0;
}

int bbutil_is_menu_open()
{
	return menuOpen;
}

void bbutil_focus()
{
	int z = 10;
	screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &z);
}

int
bbutil_init_egl(screen_context_t ctx, char *groupId, char *windowId) {
    int usage, usage_back;
    int format = SCREEN_FORMAT_RGBX8888;
    EGLint interval = 1;
    int rc, num_configs;

    EGLint attrib_list[]= { EGL_RED_SIZE,        8,
                            EGL_GREEN_SIZE,      8,
                            EGL_BLUE_SIZE,       8,
                            EGL_ALPHA_SIZE,      8,
                            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
                            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL_DEPTH_SIZE, 	 24,
                            EGL_NONE};

#ifdef USING_GL11
    usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
    attrib_list[9] = EGL_OPENGL_ES_BIT;
#elif defined(USING_GL20)
    usage = SCREEN_USAGE_OPENGL_ES2 ;
    //attrib_list[9] = EGL_OPENGL_ES2_BIT;
    EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
#else
    fprintf(stderr, "bbutil should be compiled with either USING_GL11 or USING_GL20 -D flags\n");
    return EXIT_FAILURE;
#endif

    usage_back = SCREEN_USAGE_NATIVE;

    if(egl_disp)
    {
    	int z = 10;
		screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &z);
    	int vis = 1;
    	screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);
    	//PB_eglSwapBuffers();
    	return EXIT_SUCCESS;
    }

    //Simple egl initialization
    screen_cxt = ctx;

    egl_disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (egl_disp == EGL_NO_DISPLAY)
    {
        bbutil_egl_perror("eglGetDisplay");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    //printf("eglDisplay: 0x%x", egl_disp);fflush(stdout);

    rc = eglInitialize(egl_disp, NULL, NULL);
    if (rc != EGL_TRUE)
    {
        bbutil_egl_perror("eglInitialize");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    rc = eglBindAPI(EGL_OPENGL_ES_API);

    if (rc != EGL_TRUE)
    {
        bbutil_egl_perror("eglBindApi");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    /*EGLint numConfigOut[1];
    eglChooseConfig(egl_disp, attrib_list, NULL, 0, numConfigOut);
    EGLConfig* configs = malloc(numConfigOut[0] * sizeof(EGLConfig));
    eglChooseConfig(egl_disp, attrib_list, configs, numConfigOut[0], NULL);
    int k = 0;
    for (; k < numConfigOut[0]; k++)
    {
        EGLint value;
        printf("egl-config>");
        eglGetConfigAttrib(egl_disp, configs[k], EGL_CONFIG_CAVEAT, &value);
        printf(" caveat -> %d", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_RED_SIZE, &value);
        printf(", pallette -> (%d", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_GREEN_SIZE, &value);
        printf(" %d", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_BLUE_SIZE, &value);
        printf(" %d", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_ALPHA_SIZE, &value);
        printf(" %d),", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_DEPTH_SIZE, &value);
        printf(" depth -> %d,", value);
        eglGetConfigAttrib(egl_disp, configs[k], EGL_BUFFER_SIZE, &value);
        printf(" buffer -> %d\n", value);
    }
    free(configs);*/

    if(!eglChooseConfig(egl_disp, attrib_list, &egl_conf, 1, &num_configs))
    {
        bbutil_terminate();
        return EXIT_FAILURE;
    }

#ifdef USING_GL20
        egl_ctx = eglCreateContext(egl_disp, egl_conf, EGL_NO_CONTEXT, attributes);
#elif defined(USING_GL11)
        egl_ctx = eglCreateContext(egl_disp, egl_conf, EGL_NO_CONTEXT, NULL);
#endif

    if (egl_ctx == EGL_NO_CONTEXT)
    {
        bbutil_egl_perror("eglCreateContext");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    if (use_hdmi)
        rc = screen_create_window(&screen_win, screen_cxt);
    else
        rc = screen_create_window_type(&screen_win, screen_cxt, SCREEN_CHILD_WINDOW);
    if (rc)
    {
        fprintf(stderr, "screen_create_window\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (margin_left || margin_bottom)
    {
        if (use_hdmi)
            rc = screen_create_window(&back_screen, screen_cxt);
        else
            rc = screen_create_window_type(&back_screen, screen_cxt, SCREEN_CHILD_WINDOW);
        //rc = screen_create_window(&screen_win, screen_cxt);
        if (rc) {
            fprintf(stderr, "screen_create_window - back_screen\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        rc = screen_create_window_type(&screen_win_overlay, screen_cxt, SCREEN_CHILD_WINDOW);
        //rc = screen_create_window(&screen_win, screen_cxt);
        if (rc)
        {
            fprintf(stderr, "screen_create_window - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    if (use_gamepad)
    {
        if (use_hdmi)
        {
            if (screen_set_window_property_cv(screen_win_overlay, SCREEN_PROPERTY_ID_STRING, strlen(windowId), windowId) != 0)
            {
                fprintf(stderr, "SCREEN_PROPERTY_ID_STRING - screen_win_overlay\n");
                bbutil_terminate();
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (screen_set_window_property_cv(screen_win, SCREEN_PROPERTY_ID_STRING, strlen(windowId), windowId) != 0)
            {
                fprintf(stderr, "SCREEN_PROPERTY_ID_STRING\n");
                bbutil_terminate();
                return EXIT_FAILURE;
            }
            if (margin_left || margin_bottom)
            {
                if (screen_set_window_property_cv(back_screen, SCREEN_PROPERTY_ID_STRING, strlen(windowId), windowId) != 0)
                {
                    fprintf(stderr, "SCREEN_PROPERTY_ID_STRING - back_screen\n");
                    bbutil_terminate();
                    return EXIT_FAILURE;
                }
            }
        }
    }

    if (!use_hdmi)
    {
        if (screen_join_window_group(screen_win, groupId) != 0)
        {
            fprintf(stderr, "screen_join_window_group\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
        if (margin_left || margin_bottom)
        {
            if (screen_join_window_group(back_screen, groupId) != 0)
            {
                fprintf(stderr, "screen_join_window_group - back_screen\n");
                bbutil_terminate();
                return EXIT_FAILURE;
            }
        }
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        if (screen_join_window_group(screen_win_overlay, groupId) != 0)
        {
            fprintf(stderr, "screen_join_window_group - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }


    int ndisplays;
    screen_get_context_property_iv(screen_cxt, SCREEN_PROPERTY_DISPLAY_COUNT, &ndisplays);
    screen_display_t *screen_dpy = calloc(ndisplays, sizeof(screen_display_t));
    screen_get_context_property_pv(screen_cxt, SCREEN_PROPERTY_DISPLAYS, (void**)screen_dpy);
    int i;
    for (i = 0; i < ndisplays; i++)
    {
        int type;
        screen_get_display_property_iv(screen_dpy[i], SCREEN_PROPERTY_TYPE, &type);
        if (type == 30311)
        {
            if (use_hdmi)
            {
                screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void**)&screen_dpy[i]);
                if (margin_left || margin_bottom)
                    screen_set_window_property_pv(back_screen, SCREEN_PROPERTY_DISPLAY, (void**)&screen_dpy[i]);
            }
        }
        else if (type == 30304)
        {
            if (use_hdmi)
                screen_set_window_property_pv(screen_win_overlay, SCREEN_PROPERTY_DISPLAY, (void**)&screen_dpy[i]);
            int screen_resolution[2];

            rc = screen_get_display_property_iv(screen_dpy[i], SCREEN_PROPERTY_SIZE, screen_resolution);
            if (screen_resolution[0] > screen_resolution[1]) {
                DISPLAY_WIDTH = screen_resolution[0];
                DISPLAY_HEIGHT = screen_resolution[1];
            }
            else {
                DISPLAY_WIDTH = screen_resolution[1];
                DISPLAY_HEIGHT = screen_resolution[0];
            }
            if (hdmi_width < 0)
                hdmi_width = DISPLAY_WIDTH;
            if (hdmi_height < 0)
                hdmi_height = DISPLAY_HEIGHT;
            printf("WIDTH: %d\nHEIGHT: %d\n", (int)DISPLAY_WIDTH, (int)DISPLAY_HEIGHT);
        }
    }
    free(screen_dpy);

    /*if (screen_create_window_group(screen_win, "gamewindowgroup") != 0) {
    	perror("screen_create_window_group");
    	bbutil_terminate();
    	return EXIT_FAILURE;
    }*/

    rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);
    if (rc) {
    	fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (margin_left || margin_bottom)
    {
        rc = screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_FORMAT, &format);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT) - back_screen\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        rc = screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_FORMAT, &format);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT) - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
    if (rc) {
    	fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (margin_left || margin_bottom)
    {
        rc = screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_USAGE, &usage_back);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE) - back_screen\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        rc = screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_USAGE, &usage);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE) - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    rc = screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void **)&screen_disp);
    if (rc) {
    	fprintf(stderr, "screen_get_window_property_pv(SCREEN_PROPERTY_DISPLAY)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    if (rc) {
    	fprintf(stderr, "screen_get_display_property_iv\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    //int angle = atoi(getenv("ORIENTATION"));
    //int angle = 90;

    screen_display_mode_t screen_mode;
    rc = screen_get_display_property_pv(screen_disp, SCREEN_PROPERTY_MODE, (void**)&screen_mode);
    if (rc) {
    	fprintf(stderr, "screen_get_display_property_pv(SCREEN_PROPERTY_MODE)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    /*if (margin_left || margin_bottom)
    {
        screen_display_t screen_disp2;
        screen_get_window_property_pv(screen_win_overlay, SCREEN_PROPERTY_DISPLAY, (void **)&screen_disp2);
        int trans = SCREEN_TRANSPARENCY_SOURCE_OVER;
        screen_set_display_property_iv(screen_disp2, SCREEN_PROPERTY_TRANSPARENCY, &trans);
    }*/

    int buffer_size[2] = {hdmi_width - (margin_left << 1), hdmi_height - (margin_bottom << 1)};
    int buffer_back[2] = {hdmi_width, hdmi_height};
    int buffer_overlay[2] = {DISPLAY_WIDTH, DISPLAY_HEIGHT};

/*
    if ((angle == 0) || (angle == 180)) {
        if (((screen_mode.width > screen_mode.height) && (size[0] < size[1])) ||
            ((screen_mode.width < screen_mode.height) && (size[0] > size[1]))) {
                buffer_size[1] = size[0];
                buffer_size[0] = size[1];
        }
    } else if ((angle == 90) || (angle == 270)){
        if (((screen_mode.width > screen_mode.height) && (size[0] > size[1])) ||
            ((screen_mode.width < screen_mode.height && size[0] < size[1]))) {
                buffer_size[1] = size[0];
                buffer_size[0] = size[1];
        }
    } else {
         fprintf(stderr, "Navigator returned an unexpected orientation angle.\n");
         bbutil_terminate();
         return EXIT_FAILURE;
    }
    */

    rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
    if (rc) {
    	fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (use_hdmi)
    {
        rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, buffer_size);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SIZE)\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }
    if (margin_left || margin_bottom)
    {
        rc = screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_BUFFER_SIZE, buffer_back);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE) - back_screen\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }

        screen_buffer_t screen_buf = NULL;
        screen_create_window_buffers(back_screen, 1);
        screen_get_window_property_pv(back_screen, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&screen_buf);
        int attribs[] = { SCREEN_BLIT_COLOR, 0xff000000, SCREEN_BLIT_END };
        int rect[] = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };
        screen_fill(ctx, screen_buf, attribs);
        screen_post_window(back_screen, screen_buf, 1, rect, 0);

        int margins[] = { margin_left, margin_bottom };
        rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, margins);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_POSITION)\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        rc = screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_BUFFER_SIZE, buffer_overlay);
        if (rc) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE) - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    int size[2];
    rc = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, size);
    printf("WIDTH2: %d\nHEIGHT2: %d\n", size[0], size[1]);
    if (rc) {
        fprintf(stderr, "screen_get_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE)\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    int z = 1;
    if (margin_left || margin_bottom)
        z = 2;
	if (screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &z) != 0) {
		fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_ZORDER)\n");
		return EXIT_FAILURE;
	}
	if (margin_left || margin_bottom)
	{
	    z = 1;
        if (screen_set_window_property_iv(back_screen, SCREEN_PROPERTY_ZORDER, &z) != 0) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_ZORDER) - back_screen\n");
            return EXIT_FAILURE;
        }
	}
	if (/*margin_left || margin_bottom || */use_hdmi)
	{
	    z = 2;
        if (screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_ZORDER, &z) != 0) {
            fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_ZORDER) - screen_win_overlay\n");
            return EXIT_FAILURE;
        }
	}
/*
    rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ROTATION, &angle);
    if (rc) {
        perror("screen_set_window_property_iv");
        bbutil_terminate();
        return EXIT_FAILURE;
    }*/

    rc = screen_create_window_buffers(screen_win, nbuffers);
    if (rc) {
    	fprintf(stderr, "screen_create_window_buffers\n");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        rc = screen_create_window_buffers(screen_win_overlay, nbuffers);
        if (rc) {
            fprintf(stderr, "screen_create_window_buffers - screen_win_overlay\n");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    if (q10_rotate != 0)
    {
        int rotate = q10_rotate * 90;
        while (rotate < 0)
            rotate += 360;
        if (use_hdmi)
            screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_ROTATION, &rotate);
        else
            screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ROTATION, &rotate);
    }

    if (dbg_fps)
    {
        int fps = SCREEN_DEBUG_GRAPH_FPS;
        if (screen_win_overlay)
            screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_DEBUG, &fps);
        else
            screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_DEBUG, &fps);
    }

    /*if (use_hdmi)
    {
        int scaling = SCREEN_QUALITY_FASTEST;
        screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SCALE_QUALITY, &scaling);
    }*/


    egl_surf = eglCreateWindowSurface(egl_disp, egl_conf, screen_win, NULL);
    if (egl_surf == EGL_NO_SURFACE) {
        bbutil_egl_perror("eglCreateWindowSurface");
        bbutil_terminate();
        return EXIT_FAILURE;
    }
    if (/*margin_left || margin_bottom || */use_hdmi)
    {
        egl_surf_overlay = eglCreateWindowSurface(egl_disp, egl_conf, screen_win_overlay, NULL);
        if (egl_surf == EGL_NO_SURFACE) {
            bbutil_egl_perror("eglCreateWindowSurface - screen_win_overlay");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
    }

    if (!use_overlay && use_hdmi)
    {
        eglMakeCurrent(egl_disp, egl_surf_overlay, egl_surf_overlay, egl_ctx);
        glClearColor(0, 0, 0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(egl_disp, egl_surf_overlay);
    }

    rc = eglMakeCurrent(egl_disp, egl_surf, egl_surf, egl_ctx);
    if (rc != EGL_TRUE) {
        bbutil_egl_perror("eglMakeCurrent");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    rc = eglSwapInterval(egl_disp, interval);
    if (rc != EGL_TRUE) {
        bbutil_egl_perror("eglSwapInterval");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    int vis = 1;
	screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);

	int idleMode = SCREEN_IDLE_MODE_KEEP_AWAKE;
	screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_IDLE_MODE, &idleMode);
	if (screen_win_overlay)
	    screen_set_window_property_iv(screen_win_overlay, SCREEN_PROPERTY_IDLE_MODE, &idleMode);

    initialized = 1;

    return EXIT_SUCCESS;
}

void
bbutil_terminate() {
    //Typical EGL cleanup
    if (egl_disp != EGL_NO_DISPLAY) {
        eglMakeCurrent(egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (egl_surf != EGL_NO_SURFACE) {
            eglDestroySurface(egl_disp, egl_surf);
            egl_surf = EGL_NO_SURFACE;
        }
        if (egl_ctx != EGL_NO_CONTEXT) {
            eglDestroyContext(egl_disp, egl_ctx);
            egl_ctx = EGL_NO_CONTEXT;
        }
        if (screen_win != NULL) {
            if (!use_hdmi)
                screen_leave_window_group(screen_win);
            screen_destroy_window(screen_win);
            screen_win = NULL;
        }
        if (back_screen != NULL) {
            if (!use_hdmi)
                screen_leave_window_group(back_screen);
            screen_destroy_window(back_screen);
            back_screen = 0;
        }
        if (screen_win_overlay != NULL) {
            screen_leave_window_group(screen_win_overlay);
            screen_destroy_window(screen_win_overlay);
            screen_win_overlay = NULL;
        }
        eglTerminate(egl_disp);
        egl_disp = EGL_NO_DISPLAY;
    }
    eglReleaseThread();

    initialized = 0;
}

void
bbutil_swap() {
    int rc = eglSwapBuffers(egl_disp, egl_surf);
    if (rc != EGL_TRUE) {
        bbutil_egl_perror("eglSwapBuffers");
    }
}

///* Finds the next power of 2 */
static inline int
nextp2(int x)
{
    int val = 1;
    while(val < x) val <<= 1;
    return val;
}

font_t* bbutil_load_font(const char* path, int point_size, int dpi) {
    FT_Library library;
    FT_Face face;
    int c;
    int i, j;
    font_t* font;

    if (!path){
        fprintf(stderr, "Invalid path to font file\n");
        return NULL;
    }

    if(FT_Init_FreeType(&library)) {
        fprintf(stderr, "Error loading Freetype library\n");
        return NULL;
    }
    if (FT_New_Face(library, path,0,&face)) {
        fprintf(stderr, "Error loading font %s\n", path);
        return NULL;
    }

    if(FT_Set_Char_Size ( face, point_size * 64, point_size * 64, dpi, dpi)) {
        fprintf(stderr, "Error initializing character parameters\n");
        return NULL;
    }

    font = (font_t*) malloc(sizeof(font_t));

    if (!font) {
        fprintf(stderr, "Unable to allocate memory for font structure\n");
        return NULL;
    }

    font->initialized = 0;
    font->pt = point_size;

    glGenTextures(1, &(font->font_texture));

    //Let each glyph reside in 32x32 section of the font texture
    int segment_size_x = 0, segment_size_y = 0;
    int num_segments_x = 16;
    int num_segments_y = 8;

    FT_GlyphSlot slot;
    FT_Bitmap bmp;
    int glyph_width, glyph_height;

    //First calculate the max width and height of a character in a passed font
    for(c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "FT_Load_Char failed\n");
            free(font);
            return NULL;
        }

        slot = face->glyph;
        bmp = slot->bitmap;

        glyph_width = bmp.width;
        glyph_height = bmp.rows;

        if (glyph_width > segment_size_x) {
            segment_size_x = glyph_width;
        }

        if (glyph_height > segment_size_y) {
            segment_size_y = glyph_height;
        }
    }

    int font_tex_width = nextp2(num_segments_x * segment_size_x);
    int font_tex_height = nextp2(num_segments_y * segment_size_y);

    int bitmap_offset_x = 0, bitmap_offset_y = 0;

    GLubyte* font_texture_data = (GLubyte*) calloc(2 * font_tex_width * font_tex_height, sizeof(GLubyte));

    if (!font_texture_data) {
        fprintf(stderr, "Failed to allocate memory for font texture\n");
        free(font);
        return NULL;
    }

    // Fill font texture bitmap with individual bmp data and record appropriate size, texture coordinates and offsets for every glyph
    for(c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "FT_Load_Char failed\n");
            free(font);
            return NULL;
        }

        slot = face->glyph;
        bmp = slot->bitmap;

        glyph_width = bmp.width;
        glyph_height = bmp.rows;

        div_t temp = div(c, num_segments_x);

        bitmap_offset_x = segment_size_x * temp.rem;
        bitmap_offset_y = segment_size_y * temp.quot;

        for (j = 0; j < glyph_height; j++) {
            for (i = 0; i < glyph_width; i++) {
                font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 0] =
                font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 1] =
                    (i >= bmp.width || j >= bmp.rows)? 0 : bmp.buffer[i + bmp.width * j];
            }
        }

        font->advance[c] = (float)(slot->advance.x >> 6);
        font->tex_x1[c] = (float)bitmap_offset_x / (float) font_tex_width;
        font->tex_x2[c] = (float)(bitmap_offset_x + bmp.width) / (float)font_tex_width;
        font->tex_y1[c] = (float)bitmap_offset_y / (float) font_tex_height;
        font->tex_y2[c] = (float)(bitmap_offset_y + bmp.rows) / (float)font_tex_height;
        font->width[c] = bmp.width;
        font->height[c] = bmp.rows;
        font->offset_x[c] = (float)slot->bitmap_left;
        font->offset_y[c] =  (float)((slot->metrics.horiBearingY-face->glyph->metrics.height) >> 6);
    }

    glBindTexture(GL_TEXTURE_2D, font->font_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, font_tex_width, font_tex_height, 0, GL_LUMINANCE_ALPHA , GL_UNSIGNED_BYTE, font_texture_data);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_tex_width, font_tex_height, 0, GL_RGBA , GL_UNSIGNED_BYTE, font_texture_data);

    free(font_texture_data);

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    font->initialized = 1;
    return font;
}

void bbutil_render_text(font_t* font, const char* msg, float x, float y, float r, float g, float b, float a) {
    int i, c;
    GLfloat *vertices;
    GLfloat *texture_coords;
    GLshort* indices;

    float pen_x = 0.0f;

    if (!font) {
        fprintf(stderr, "Font must not be null\n");
        return;
    }

    if (!font->initialized) {
        fprintf(stderr, "Font has not been loaded\n");
        return;
    }

    if (!msg) {
        return;
    }

    const int msg_len = strlen(msg);

    vertices = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);
    texture_coords = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);

    indices = (GLshort*) malloc(sizeof(GLfloat) * 6 * msg_len);

    for(i = 0; i < msg_len; ++i) {
        c = msg[i];

        vertices[8 * i + 0] = x + pen_x + font->offset_x[c];
        vertices[8 * i + 1] = y + font->offset_y[c];
        vertices[8 * i + 2] = vertices[8 * i + 0] + font->width[c];
        vertices[8 * i + 3] = vertices[8 * i + 1];
        vertices[8 * i + 4] = vertices[8 * i + 0];
        vertices[8 * i + 5] = vertices[8 * i + 1] + font->height[c];
        vertices[8 * i + 6] = vertices[8 * i + 2];
        vertices[8 * i + 7] = vertices[8 * i + 5];

        texture_coords[8 * i + 0] = font->tex_x1[c];
        texture_coords[8 * i + 1] = font->tex_y2[c];
        texture_coords[8 * i + 2] = font->tex_x2[c];
        texture_coords[8 * i + 3] = font->tex_y2[c];
        texture_coords[8 * i + 4] = font->tex_x1[c];
        texture_coords[8 * i + 5] = font->tex_y1[c];
        texture_coords[8 * i + 6] = font->tex_x2[c];
        texture_coords[8 * i + 7] = font->tex_y1[c];

        indices[i * 6 + 0] = 4 * i + 0;
        indices[i * 6 + 1] = 4 * i + 1;
        indices[i * 6 + 2] = 4 * i + 2;
        indices[i * 6 + 3] = 4 * i + 2;
        indices[i * 6 + 4] = 4 * i + 1;
        indices[i * 6 + 5] = 4 * i + 3;

        //Assume we are only working with typewriter fonts
        pen_x += font->advance[c];
    }
#ifdef USING_GL11
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glColor4f(r, g, b, a);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
    glBindTexture(GL_TEXTURE_2D, font->font_texture);

    glDrawElements(GL_TRIANGLES, 6 * msg_len, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
#elif defined USING_GL20
    if (!text_program_initialized) {
        GLint status;

        // Create shaders if this hasn't been done already
        const char* v_source =
                "precision mediump float;"
                "attribute vec2 a_position;"
                "attribute vec2 a_texcoord;"
                "varying vec2 v_texcoord;"
                "void main()"
                "{"
                "   gl_Position = vec4(a_position, 0.0, 1.0);"
                "    v_texcoord = a_texcoord;"
                "}";

        const char* f_source =
                "precision lowp float;"
                "varying vec2 v_texcoord;"
                "uniform sampler2D u_font_texture;"
                "uniform vec4 u_col;"
                "void main()"
                "{"
                "    vec4 temp = texture2D(u_font_texture, v_texcoord);"
                "    gl_FragColor = u_col * temp;"
                "}";

        // Compile the vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);

        if (!vs) {
            fprintf(stderr, "Failed to create vertex shader: %d\n", glGetError());
            return;
        } else {
            glShaderSource(vs, 1, &v_source, 0);
            glCompileShader(vs);
            glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
            if (GL_FALSE == status) {
                GLchar log[256];
                glGetShaderInfoLog(vs, 256, NULL, log);

                fprintf(stderr, "Failed to compile vertex shader: %s\n", log);

                glDeleteShader(vs);
            }
        }

        // Compile the fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

        if (!fs) {
            fprintf(stderr, "Failed to create fragment shader: %d\n", glGetError());
            return;
        } else {
            glShaderSource(fs, 1, &f_source, 0);
            glCompileShader(fs);
            glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
            if (GL_FALSE == status) {
                GLchar log[256];
                glGetShaderInfoLog(fs, 256, NULL, log);

                fprintf(stderr, "Failed to compile fragment shader: %s\n", log);

                glDeleteShader(vs);
                glDeleteShader(fs);

                return;
            }
        }

        // Create and link the program
        text_rendering_program = glCreateProgram();
        if (text_rendering_program)
        {
            glAttachShader(text_rendering_program, vs);
            glAttachShader(text_rendering_program, fs);
            glLinkProgram(text_rendering_program);

            glGetProgramiv(text_rendering_program, GL_LINK_STATUS, &status);
            if (status == GL_FALSE)    {
                GLchar log[256];
                glGetProgramInfoLog(fs, 256, NULL, log);

                fprintf(stderr, "Failed to link text rendering shader program: %s\n", log);

                glDeleteProgram(text_rendering_program);
                text_rendering_program = 0;

                return;
            }
        } else {
            fprintf(stderr, "Failed to create a shader program\n");

            glDeleteShader(vs);
            glDeleteShader(fs);
            return;
        }

        // We don't need the shaders anymore - the program is enough
        glDeleteShader(fs);
        glDeleteShader(vs);

        glUseProgram(text_rendering_program);

        // Store the locations of the shader variables we need later
        positionLoc = glGetAttribLocation(text_rendering_program, "a_position");
        texcoordLoc = glGetAttribLocation(text_rendering_program, "a_texcoord");
        textureLoc = glGetUniformLocation(text_rendering_program, "u_font_texture");
        colorLoc = glGetUniformLocation(text_rendering_program, "u_col");

        text_program_initialized = 1;
    }

    glEnable(GL_BLEND);

    //Map text coordinates from (0...surface width, 0...surface height) to (-1...1, -1...1)
    //this make our vertex shader very simple and also works irrespective of orientation changes
    EGLint surface_width, surface_height;

    eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
    eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

    for(i = 0; i < 4 * msg_len; ++i) {
        vertices[2 * i + 0] = 2 * vertices[2 * i + 0] / surface_width - 1.0f;
        vertices[2 * i + 1] = 2 * vertices[2 * i + 1] / surface_height - 1.0f;
    }

    //Render text
    glUseProgram(text_rendering_program);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->font_texture);
    glUniform1i(textureLoc, 0);

    glUniform4f(colorLoc, r, g, b, a);

    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glEnableVertexAttribArray(texcoordLoc);
    glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);

       //Draw the string
    glDrawElements(GL_TRIANGLES, 6 * msg_len, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(positionLoc);
    glDisableVertexAttribArray(texcoordLoc);
#else
    fprintf(stderr, "bbutil should be compiled with either USING_GL11 or USING_GL20 -D flags\n");
#endif

    free(vertices);
    free(texture_coords);
    free(indices);
}

void bbutil_destroy_font(font_t* font) {
    if (!font) {
        return;
    }

    glDeleteTextures(1, &(font->font_texture));

    free(font);
}

void bbutil_measure_text(font_t* font, const char* msg, float* width, float* height) {
    int i, c;

    if (!msg) {
        return;
    }

    const int msg_len  =strlen(msg);

    if (width) {
        //Width of a text rectangle is a sum advances for every glyph in a string
        *width = 0.0f;

        for(i = 0; i < msg_len; ++i) {
            c = msg[i];
            *width += font->advance[c];
        }
    }

    if (height) {
        //Height of a text rectangle is a high of a tallest glyph in a string
        *height = 0.0f;

        for(i = 0; i < msg_len; ++i) {
            c = msg[i];

            if (*height < font->height[c]) {
                *height = font->height[c];
            }
        }
    }
}

int bbutil_load_texture(const char* filename, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex, png_byte **raw_pix) {
    int i;
    GLuint format;
    //header for testing if it is a png
    png_byte header[8];

    if (!tex) {
        return EXIT_FAILURE;
    }

    //open file as binary
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return EXIT_FAILURE;
    }

    //read the header
    fread(header, 1, 8, fp);

    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(fp);
        return EXIT_FAILURE;
    }

    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return EXIT_FAILURE;
    }

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        return EXIT_FAILURE;
    }

    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return EXIT_FAILURE;
    }

    //setup error handling (required without using custom error handlers above)
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return EXIT_FAILURE;
    }

    //init png reading
    png_init_io(png_ptr, fp);

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 image_width, image_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &image_width, &image_height, &bit_depth, &color_type, NULL, NULL, NULL);

    switch (color_type)
    {
        case PNG_COLOR_TYPE_RGBA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = GL_RGB;
            break;
        default:
            fprintf(stderr,"Unsupported PNG color type (%d) for texture: %s", (int)color_type, filename);
            fclose(fp);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return NULL;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = (png_byte*) malloc(sizeof(png_byte) * rowbytes * image_height);

    if (!image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return EXIT_FAILURE;
    }

    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image_height);
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return EXIT_FAILURE;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (i = 0; i < image_height; i++) {
        row_pointers[image_height - 1 - i] = image_data + i * rowbytes;
    }

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    int tex_width, tex_height;

    tex_width = nextp2(image_width);
    tex_height = nextp2(image_height);

    GLint err;
    if(raw_pix == 0){
		glGenTextures(1, tex);
		glBindTexture(GL_TEXTURE_2D, (*tex));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if ((tex_width != image_width) || (tex_height != image_height) ) {
			glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, NULL);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, format, GL_UNSIGNED_BYTE, image_data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, image_data);
		}

		err = glGetError();
		//png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    	free(image_data);
    } else{
    	*raw_pix = image_data;
    	err = 0;
    }

    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(row_pointers);
    fclose(fp);

    if (err == 0) {
        //Return physical with and height of texture if pointers are not null
        if(width) {
            *width = image_width;
        }
        if (height) {
            *height = image_height;
        }
        //Return modified texture coordinates if pointers are not null
        if(tex_x) {
            *tex_x = ((float) image_width - 0.5f) / ((float)tex_width);
        	//*tex_x = ((float)tex_width);
        }
        if(tex_y) {
            *tex_y = ((float) image_height - 0.5f) / ((float)tex_height);
        	//*tex_y = ((float)tex_height);
        }
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "GL error %i \n", err);
        return EXIT_FAILURE;
    }
}

int bbutil_calculate_dpi(screen_context_t ctx) {
    int rc;
    int screen_phys_size[2];

    rc = screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_PHYSICAL_SIZE, screen_phys_size);
    if (rc) {
        perror("screen_get_display_property_iv");
        bbutil_terminate();
        return EXIT_FAILURE;
    }

    //Simulator will return 0,0 for physical size of the screen, so use 170 as default dpi
    if ((screen_phys_size[0] == 0) && (screen_phys_size[1] == 0)) {
        return 170;
    } else {
        int screen_resolution[2];
        rc = screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_SIZE, screen_resolution);
        if (rc) {
            perror("screen_get_display_property_iv");
            bbutil_terminate();
            return EXIT_FAILURE;
        }
        double diagonal_pixels = sqrt(screen_resolution[0] * screen_resolution[0] + screen_resolution[1] * screen_resolution[1]);
        double diagonal_inches = 0.0393700787 * sqrt(screen_phys_size[0] * screen_phys_size[0] + screen_phys_size[1] * screen_phys_size[1]);
        return (int)(diagonal_pixels / diagonal_inches + 0.5);

    }
}



float UIPixelToViewportX(float x)
{
	return ((x/DISPLAY_WIDTH)*2.0f)-1.0f;
}

float UIPixelToViewportY(float y)
{
	return -(((y/DISPLAY_HEIGHT)*2.0f)-1.0f);
}

float UIPixelToViewportW(float w)
{
	return (w/DISPLAY_WIDTH)*2.0f;
}

float UIPixelToViewportH(float h)
{
	return (h/DISPLAY_HEIGHT)*2.0f;
}

void offset_text(UIQuad * text, float x, float y){
	int i;

	text->x += x;
	text->y += y;

	for(i = 0; i < 4 * strlen(text->text); ++i) {
		text->vertices[2 * i + 0] += UIPixelToViewportX(x) + 1.0f;
		text->vertices[2 * i + 1] += -UIPixelToViewportY(y) + 1.0f;
	}
}

UIQuad * create_text(font_t *font, float x, float y, char * msg){
	int i, c;
	float pen_x = 0.0f;

	if (!msg) {
		return NULL;
	}

	UIQuad* quad = malloc(sizeof(UIQuad));

	quad->x = x;
	quad->y = y;
	//bbutil_measure_text(font, msg, &quad->textureW, &quad->textureH);

	quad->scale = 1.0;
	quad->alpha = 0.5f;

	quad->text = malloc(strlen(msg)+1);
	strcpy(quad->text,msg);

	quad->program = text_rendering_program;

	//Create the texture

	if (!font) {
		fprintf(stderr, "Font must not be null\n");
		return NULL;
	}

	if (!font->initialized) {
		fprintf(stderr, "Font has not been loaded\n");
		return NULL;
	}

	const int msg_len = strlen(msg);

	quad->vertices = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);
	quad->texture_coords = (GLfloat*) malloc(sizeof(GLfloat) * 8 * msg_len);

	quad->indices = (GLshort*) malloc(sizeof(GLfloat) * 6 * msg_len);

	for(i = 0; i < msg_len; ++i) {
		c = msg[i];

		quad->vertices[8 * i + 0] = UIPixelToViewportX( x + pen_x + font->offset_x[c]);
		quad->vertices[8 * i + 1] = -UIPixelToViewportY( y + font->offset_y[c]);
		quad->vertices[8 * i + 2] = quad->vertices[8 * i + 0] + UIPixelToViewportX(font->width[c]) + 1.0f; //UI(x)+UI(x1)+1=UI(x+x1)
		quad->vertices[8 * i + 3] = quad->vertices[8 * i + 1];
		quad->vertices[8 * i + 4] = quad->vertices[8 * i + 0];
		quad->vertices[8 * i + 5] = quad->vertices[8 * i + 1] - UIPixelToViewportY(font->height[c]) + 1.0f;
		quad->vertices[8 * i + 6] = quad->vertices[8 * i + 2];
		quad->vertices[8 * i + 7] = quad->vertices[8 * i + 5];

		quad->texture_coords[8 * i + 0] = font->tex_x1[c];
		quad->texture_coords[8 * i + 1] = font->tex_y2[c];
		quad->texture_coords[8 * i + 2] = font->tex_x2[c];
		quad->texture_coords[8 * i + 3] = font->tex_y2[c];
		quad->texture_coords[8 * i + 4] = font->tex_x1[c];
		quad->texture_coords[8 * i + 5] = font->tex_y1[c];
		quad->texture_coords[8 * i + 6] = font->tex_x2[c];
		quad->texture_coords[8 * i + 7] = font->tex_y1[c];
		//printf("tex_coords:%f\n", font->tex_x1[c]);

		quad->indices[i * 6 + 0] = 4 * i + 0;
		quad->indices[i * 6 + 1] = 4 * i + 1;
		quad->indices[i * 6 + 2] = 4 * i + 2;
		quad->indices[i * 6 + 3] = 4 * i + 2;
		quad->indices[i * 6 + 4] = 4 * i + 1;
		quad->indices[i * 6 + 5] = 4 * i + 3;

		//Assume we are only working with typewriter fonts
		pen_x += font->advance[c];
	}
/*
	for(i = 0; i < 4 * msg_len; ++i) {
		quad->vertices[2 * i + 0] = UIPixelToViewportX(quad->vertices[2 * i + 0]);
		quad->vertices[2 * i + 1] = -UIPixelToViewportY(quad->vertices[2 * i + 1]);
	}
*/

	bbutil_measure_text(font, msg, &quad->textureW, &quad->textureH);

	return quad;
}

UIQuad *create_image(const char* filename, float x, float y,  int raw_pix){
	UIQuad* quad = malloc(sizeof(UIQuad));
	int rc, retry = 1;
	int w, h;

retry:
	if(raw_pix == 1){
		rc = bbutil_load_texture(filename, &w, &h, &quad->textureW, &quad->textureH, &quad->texture, &quad->raw_pix);
	} else {
		rc = bbutil_load_texture(filename, &w, &h, &quad->textureW, &quad->textureH, &quad->texture, 0);
	}
	if(rc == 1){
		printf("Error loading texture: %s", filename);fflush(stdout);
		if(retry){
			retry = 0;
			goto retry;
		} else {
			return NULL;
		}
	}
	quad->program = text_rendering_program;
	quad->scale = 1.0;
	quad->alpha = 1.0f;
	quad->text = NULL;

	quad->x = UIPixelToViewportX(x);
	quad->y = UIPixelToViewportY(y);
	quad->w = UIPixelToViewportW((float)w);
	quad->h = UIPixelToViewportH((float)h);
	quad->textureW = w;
	quad->textureH = h;

	printf("Image Created: w:%f, h:%f, texW:%f, texH:%f, raw:%d\n", quad->w, quad->h, quad->textureW, quad->textureH, raw_pix);fflush(stdout);

	return quad;
}

int delete_image(UIQuad *quad, int raw_pix){

	if(raw_pix){
		free(quad->raw_pix);
	} else {
		glDeleteTextures(1, &quad->texture);
	}

	free(quad);

	return 0;
}

typedef struct {
    float Position[2];
    float Texture[2];
} Vertex;

void render_text(font_t *font, UIQuad*quad){
	int i;

	//Render text
	glUseProgram(text_rendering_program);

	glBindTexture(GL_TEXTURE_2D, font->font_texture);

	glUniform1i(textureLoc, 0);
	glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, quad->alpha);

	//glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, quad->vertices);

	//glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, quad->texture_coords);

	   //Draw the string
	glDrawElements(GL_TRIANGLES, 6 * (strlen(quad->text)), GL_UNSIGNED_SHORT, quad->indices);

	//glDisableVertexAttribArray(positionLoc);
	//glDisableVertexAttribArray(texcoordLoc);
}

void render_image(UIQuad *quad){

	glUseProgram(text_rendering_program);

	glBindTexture(GL_TEXTURE_2D, quad->texture);

	glUniform1i(textureLoc, 0);
	glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, quad->alpha);

	float s = max(quad->scale,0.0f) * 0.5f;
	float scaledW = quad->w * s;
	float scaledH = quad->h * s;
	float midX = quad->x + (quad->w / 2.0f) + (quad->w * (s-0.5f));
	float midY = quad->y - (quad->h / 2.0f) - (quad->h * (s-0.5f));


	//printf("Render:\nMidx:%f\nMidy:%f\nscaledW:%f\nscaledH:%f\n", midX, midY, scaledW, scaledH);fflush(stdout);
	GLfloat vertices[] =
	{
		//x					y        					u        v
		midX+scaledW,	-(midY+scaledH), 					1.0,	 +0.0,
		midX+scaledW, 	-(midY-scaledH), 					1.0,	 1.0,
		midX-scaledW,	-(midY-scaledH), 					+0.0,	 1.0,
		midX-scaledW,	-(midY+scaledH), 					0.0,	+0.0,
	};

	GLshort indices[] = {
		0,1,2,
		2,3,0,
	};

	//glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (float*)vertices);

	//glEnableVertexAttribArray(texcoordLoc);
	glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (float*)vertices+2);

	//Draw the string
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0 , 4);

	//glDisableVertexAttribArray(positionLoc);
	//glDisableVertexAttribArray(texcoordLoc);
}

void compile_text_program(){
	GLint status;

	// Create shaders if this hasn't been done already
	const char* v_source =
			"precision mediump float;"
			"attribute vec2 a_position;"
			"attribute vec2 a_texcoord;"
			"varying vec2 v_texcoord;"
			"void main()"
			"{"
			"   gl_Position = vec4(a_position, 0.0, 1.0);"
			"    v_texcoord = a_texcoord;"
			"}";

	const char* f_source =
			"precision lowp float;"
			"varying vec2 v_texcoord;"
			"uniform sampler2D u_font_texture;"
			"uniform vec4 u_col;"
			"void main()"
			"{"
			"    vec4 temp = texture2D(u_font_texture, v_texcoord);"
			"    gl_FragColor = u_col * temp;"
			"}";


	// Compile the vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	if (!vs) {
		fprintf(stderr, "Failed to create vertex shader: %d\n", glGetError());
		return;
	} else {
		glShaderSource(vs, 1, &v_source, 0);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (GL_FALSE == status) {
			GLchar log[256];
			glGetShaderInfoLog(vs, 256, NULL, log);

			fprintf(stderr, "Failed to compile vertex shader: %s\n", log);

			glDeleteShader(vs);
		}
	}

	// Compile the fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	if (!fs) {
		fprintf(stderr, "Failed to create fragment shader: %d\n", glGetError());
		return;
	} else {
		glShaderSource(fs, 1, &f_source, 0);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (GL_FALSE == status) {
			GLchar log[256];
			glGetShaderInfoLog(fs, 256, NULL, log);

			fprintf(stderr, "Failed to compile fragment shader: %s\n", log);

			glDeleteShader(vs);
			glDeleteShader(fs);

			return;
		}
	}

	// Create and link the program
	text_rendering_program = glCreateProgram();
	if (text_rendering_program)
	{
		glAttachShader(text_rendering_program, vs);
		glAttachShader(text_rendering_program, fs);
		glLinkProgram(text_rendering_program);

		glGetProgramiv(text_rendering_program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)    {
			GLchar log[256];
			glGetProgramInfoLog(fs, 256, NULL, log);

			fprintf(stderr, "Failed to link text rendering shader program: %s\n", log);

			glDeleteProgram(text_rendering_program);
			text_rendering_program = 0;

			return;
		}
	} else {
		fprintf(stderr, "Failed to create a shader program\n");

		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}

	// We don't need the shaders anymore - the program is enough
	glDeleteShader(fs);
	glDeleteShader(vs);

		// Store the locations of the shader variables we need later
		positionLoc = glGetAttribLocation(text_rendering_program, "a_position");
		texcoordLoc = glGetAttribLocation(text_rendering_program, "a_texcoord");
		textureLoc = glGetUniformLocation(text_rendering_program, "u_font_texture");
		colorLoc = glGetUniformLocation(text_rendering_program, "u_col");

	return;
}

#define MAXPATHLEN 256
enum VIDEO_PLUGIN
{
	VIDEO_PLUGIN_RICE,
	VIDEO_PLUGIN_GLES2N64,
	VIDEO_PLUGIN_GLIDE64
};

void set_z_order(int z){
	int zOrder = z;

	if (screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &zOrder) != 0) {
		return;
	}
}

void write_bitmap_header(int nbytes, int fd, int width, int height)
{
	char header[54];
	header[0] = 'B';
	header[1] = 'M';
	header[2] = nbytes & 0xff;
	header[3] = (nbytes >> 8) & 0xff;
	header[4] = (nbytes >> 16) & 0xff;
	header[5] = (nbytes >> 24) & 0xff;
	header[6] = 0;
	header[7] = 0;
	header[8] = 0;
	header[9] = 0;
	header[10] = 54;
	header[11] = 0;
	header[12] = 0;
	header[13] = 0;
	header[14] = 40;
	header[15] = 0;
	header[16] = 0;
	header[17] = 0;
	header[18] = width & 0xff;
	header[19] = (width >> 8) & 0xff;
	header[20] = (width >> 16) & 0xff;
	header[21] = (width >> 24) & 0xff;
	header[22] = -height & 0xff;
	header[23] = (-height >> 8) & 0xff;
	header[24] = (-height >> 16) & 0xff;
	header[25] = (-height >> 24) & 0xff;
	header[26] = 1;
	header[27] = 0;
	header[28] = 32;
	header[29] = 0;
	header[30] = 0;
	header[31] = 0;
	header[32] = 0;
	header[33] = 0;
	header[34] = 0; /* image size*/
	header[35] = 0;
	header[36] = 0;
	header[37] = 0;
	header[38] = 0x9;
	header[39] = 0x88;
	header[40] = 0;
	header[41] = 0;
	header[42] = 0x9l;
	header[43] = 0x88;
	header[44] = 0;
	header[45] = 0;
	header[46] = 0;
	header[47] = 0;
	header[48] = 0;
	header[49] = 0;
	header[50] = 0;
	header[51] = 0;
	header[52] = 0;
	header[53] = 0;

	/* Write bitmap header to file */
	write(fd, header, sizeof(header));
}

extern int creat(const char*, mode_t);

void write_bitmap_file(const char* filename, int width, int height, const char* screenshot_ptr, const int screenshot_stride)
{
	int nbytes;
	int fd;
	int i;
	nbytes = width * height * 4;
	fd = creat(filename, S_IRUSR | S_IWUSR);
	write_bitmap_header(nbytes, fd, width, height);
	for (i = 0; i < height; i++)
		write(fd, screenshot_ptr + i * screenshot_stride, width * 4);
}

int bbutil_screencapture(const char* filename, int x, int y, int width, int height)
{
	screen_pixmap_t screen_pix;
	screen_buffer_t screenshot_buf;

	char *screenshot_ptr = 0;
	int screenshot_stride = 0;
	int usage, format;
	screen_create_pixmap(&screen_pix, screen_cxt);
	usage = SCREEN_USAGE_READ | SCREEN_USAGE_NATIVE;
	screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &usage);
	format = SCREEN_FORMAT_RGBA8888;
	screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &format);
	int size[2];
	screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, size);
	screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, size);
	screen_create_pixmap_buffer(screen_pix);
	screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&screenshot_buf);
	screen_get_buffer_property_pv(screenshot_buf, SCREEN_PROPERTY_POINTER, (void**)&screenshot_ptr);
	screen_get_buffer_property_iv(screenshot_buf, SCREEN_PROPERTY_STRIDE, &screenshot_stride);
	screen_read_window(screen_win, screenshot_buf, 0, NULL, 0);
	screenshot_ptr += x * 4;
	write_bitmap_file(filename, width, height, screenshot_ptr, screenshot_stride);
	return 0;
}
