/* About: License
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016-2019 Syoyo Fujita and many contributors.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */
#ifndef TINOBJ_LOADER_C_H_
#define TINOBJ_LOADER_C_H_

/* @todo { Remove stddef dependency. size_t? } */
#include <stddef.h>

/* Constant: TINYOBJ_LOADER_C_IMPLEMENTATION
 * Constant used to mark the .c file that holds the implementation of tinyobj
 *
 * Only *one* file should have this definition!
 */

/* Constant: TINYOBJ_USE_UTHASH
 * Use <UThash: http://troydhanson.github.io/uthash/> hashtable implementation
 * instead of tinyobj's implementation
 */
//#define TINYOBJ_USE_UTHASH

/* Constant: TINYOBJ_ENABLE_OLDER_ATTRIBUTE
 *
 * Enable use of older attribute type for objects (deprecated)
 *
 * See:
 *  - <COMPATtinyobj_attrib_t>
 *  - <tinyobj_new2old>
 *  - <tinyobj_attrib_freeCOMPAT>
 */
#define TINYOBJ_ENABLE_OLDER_ATTRIBUTE

/* Constant: TINYOBJ_FLAG_TRIANGULATE
 *
 * Should the faces be triangulated upon loading
 * When triangulating we parse the vertices as if they were part of
 * a triangle fan, so every three indices the next is always the first
 * triplet of this face
 *
 * See:
 *  <tinyobj_obj_parse_face>
 */
#define TINYOBJ_FLAG_TRIANGULATE (1 << 0)

/* Constant: TINYOBJ_INVALID_INDEX
 *
 * Invalid vertex index used when referencing vertices in couples/tuples
 *
 * See:
 *  - <fixIndex>
 *  - <parseRawTriple>
 *  - <tinyobj_vertex_index_t>
 */
#define TINYOBJ_INVALID_INDEX (0x80000000)

/* Constants: Return codes
 *
 * TINYOBJ_NO_COMMAND                   - No command
 * TINYOBJ_SUCCESS                      - Success
 * TINYOBJ_ERROR_NOT_SET                - Default error
 * TINYOBJ_ERROR_MEMORY                 - Memory failure
 * TINYOBJ_ERROR_EMPTY                  - Empty file
 * TINYOBJ_ERROR_FILE_OPERATION         - Failed file operation
 * TINYOBJ_ERROR_INVALID_PARAMETER      - Invalid parameter (function)
 * TINYOBJ_ERROR_UNKNOWN_PARAMETER      - Unknown parameter for object/material
 * TINYOBJ_ERROR_MALFORMED_PARAMETER    - Malformed parameter for object/material
 */
#define TINYOBJ_NO_COMMAND (1)
#define TINYOBJ_SUCCESS (0)
#define TINYOBJ_ERROR_NOT_SET (-1)
#define TINYOBJ_ERROR_MEMORY (-2)
#define TINYOBJ_ERROR_EMPTY (-3)
#define TINYOBJ_ERROR_FILE_OPERATION (-4)
#define TINYOBJ_ERROR_INVALID_PARAMETER (-5)
#define TINYOBJ_ERROR_UNKNOWN_PARAMETER (-6)
#define TINYOBJ_ERROR_MALFORMED_PARAMETER (-7)

/* Constants: Dynamic array basic parameters
 *
 * Initial array lengths and growth factors
 *
 * TINYOBJ_POINT_INITIAL_COUNT     - Point array initial length
 * TINYOBJ_POINT_GROWTH_FACTOR     - Point array growth factor
 * TINYOBJ_COUPLE_INITIAL_COUNT    - Couple (line) array initial length
 * TINYOBJ_COUPLE_GROWTH_FACTOR    - Couple (line) array growth factor
 * TINYOBJ_TRIPLET_INITIAL_COUNT   - Triplet (face) array initial length
 * TINYOBJ_TRIPLET_GROWTH_FACTOR   - Triplet (face) array growth factor
 * TINYOBJ_MATERIAL_INITIAL_COUNT  - Material array initial length
 * TINYOBJ_MATERIAL_GROWTH_FACTOR  - Material array growth factor
 */
#define TINYOBJ_POINT_INITIAL_COUNT (16)
#define TINYOBJ_POINT_GROWTH_FACTOR (2)
#define TINYOBJ_COUPLE_INITIAL_COUNT (16)
#define TINYOBJ_COUPLE_GROWTH_FACTOR (2)
#define TINYOBJ_TRIPLET_INITIAL_COUNT (16)
#define TINYOBJ_TRIPLET_GROWTH_FACTOR (2)
#define TINYOBJ_MATERIAL_INITIAL_COUNT (2)
#define TINYOBJ_MATERIAL_GROWTH_FACTOR (2)

/***********************************************************************************************
 * Group: Vertex data
 ***********************************************************************************************/

/* Structure: tinyobj_vertex_t
 *
 * Geometric vertex (v)
 *  Specifies a geometric vertex and its x y z coordinates. Rational
 *  curves and surfaces require a fourth homogeneous coordinate, also
 *  called the weight.
 * > v  x y z w
 *
 * Fields:
 *
 *  x      - x coordinate
 *  y      - y coordinate
 *  z      - z coordinate
 *  weight - (Rational curves/surfaces) Weight (default: 1.0f)
 */
typedef struct s_tinyobj_vertex {
  float x, y, z;  //< Coordinates
  float weight;   //< (Rational curves/surfaces) Weight (default: 1.0f)
} tinyobj_vertex_t;

/* Structure: tinyobj_vertex_normal_t
 *
 * Vertex normal (vn)
 *  Specifies a normal vector with components i, j, and k
 * > vn  i j k
 *
 * Fields:
 *
 *  i      - i coordinate
 *  j      - j coordinate
 *  k      - k coordinate
 */
typedef struct s_tinyobj_vertex_normal {
  float i, j, k;  //< Coordinates
} tinyobj_vertex_normal_t;

/* Structure: tinyobj_vertex_texture_t
 *
 * Texture vertex (vt)
 *  Specifies a texture vertex and its coordinates
 * > vt  u v w
 *
 * Fields:
 *
 *  u      - Horizontal direction
 *  v      - (2D and 3D) Vertical direction (Default: 0)
 *  w      - (3D) Depth (Default: 0)
 */
typedef struct s_tinyobj_vertex_texture {
  float u;  //< Horizontal direction
  float v;  //< (2D and 3D) Vertical direction (Default: 0)
  float w;  //< (3D) Depth (Default: 0)
} tinyobj_vertex_texture_t;

/* Structure: tinyobj_vertex_param_t
 *
 * Parameter space vertex (vp)
 *  Specifies a point in the parameter space of a curve or surface.
 * > vp  u v w
 *
 * Fields:
 *
 * u      - (1D) Space control point
 * v      - (2D) Space control point
 * weight - (Rational trimming curves) Weight (default: 0)
 */
typedef struct s_tinyobj_vertex_param {
  float u, v;
  float weight;
} tinyobj_vertex_param_t;

/***********************************************************************************************
 * Group: Element data
 *  Polygonal and free-form geometry
 ***********************************************************************************************/

/* Structure: tinyobj_vertex_index_t
 * Indices for each vertex type, used for triplets in faces
 * See:
 *  <tinyobj_face_t>
 *
 * Fields:
 *
 * v_idx  - Geometric vertex index
 * vt_idx - Texture vertex index
 * vn_idx - Normal vertex index
 */
typedef struct {
  int v_idx, vt_idx, vn_idx;
} tinyobj_vertex_index_t;

/* Structure: tinyobj_point_t
 * Point (p)
 *  Specifies a point element and its vertex
 * > p  v1 v2 v3
 *
 * Fields:
 *
 * v_idx  - List of vertices
 * count  -	Point count
 * length -	List length
 */
typedef struct s_tinyobj_point {
  int *v_idx;     //< List of vertices
  size_t count;   //< Point count
  size_t length;  //< List length
} tinyobj_point_t;

/* Structure: tinyobj_line_t
 * Line (l)
 *  Specifies a line and its vertex reference numbers
 * > l  v1/vt1   v2/vt2   v3/vt3
 *
 * Fields:
 *
 * couple_list - List of couples contained in this line
 * count       - Count of couples
 * length      - Total length of couple list
 */
typedef struct s_tinyobj_line {
  /* Structure: tinyobj_line_t.s_line_vertex_index
   *  List of couples
   *
   * Fields:
   *
   * v_idx  - Geometric vertex
   * vt_idx - Texture vertex (optional)
   */
  struct s_line_vertex_index {
    int v_idx,   //< Geometric Vertex
        vt_idx;  //< Texture Vertex (optional)
  } * couple_list;
  size_t count;   //< Count of couples
  size_t length;  //< Length of couple list
} tinyobj_line_t;

/* Structure: tinyobj_face_t
 * Face (f)
 *  Specifies a face element and its vertex reference number
 * > f  v1/vt1/vn1   v2/vt2/vn2
 * > fo (deprecated)
 *
 * Fields:
 *
 * triplet list - A list of triplets contained in this face
 *
 *                *v*  Geometric vertex (minimum of 3)
 *
 *                *vt* Texture vertex (optional)
 *
 *                *vn* Vertex normal (optional)
 *
 *                When an index is empty it is equal to <TINYOBJ_INVALID_INDEX>
 * count          - Count of triplets
 * length         - Length of triplet list
 * triangle_count - Number of triangles in this face
 * material_id    - Material to be applied to this face (defaults to -1)
 * smoothing_id   - Smoothing group to be applied to this face (defaults to 0)
 */
typedef struct s_tinyobj_face {
  tinyobj_vertex_index_t *triplet_list;
  unsigned int count;           //< Count of triplets
  unsigned int length;          //< Length of triplet list
  unsigned int triangle_count;  //< Number of triangles in this face
  int material_id;   //< Material to be applied to this face (defaults to -1)
  int smoothing_id;  //< Smoothing group to be applied to this face (defaults to 0)
} tinyobj_face_t;

/* Structure: tinyobj_coefficient_t
 * RGB Color coefficient information used in materials
 * <tinyobj_material_t>
 */
typedef struct s_coefficient_information {
  float r, g, b;
} tinyobj_coefficient_t;

/* Structure: tinyobj_material_t
 * Material attribute
 * <.mtl format: http://paulbourke.net/dataformats/mtl/>
 *
 * Fields:
 *
 * name                        - Material name
 * ambient                     - Ambient color coefficient (reflectivity)
 * diffuse                     - Diffuse color coefficient (reflectivity)
 * specular                    - Specular color coefficient (reflectivity)
 * transmittance               - Transmittance color coefficient (reflectivity)
 * emission                    - Emission color coefficient (reflectivity)
 * shininess                   - Specular exponent
 * ior                         - Index Of Refraction (ior) 'optical density'
 * dissolve                    - Non-transparency to be alpha 'dissolve' (0.0f transparent-1.0f opaque)
 * illum                       - Illumination model (0-10)
 * ambient_texname             - map_Ka
 * diffuse_texname             - map_Kd
 * specular_texname            - map_Ks
 * specular_highlight_texname  - map_Ns
 * bump_texname                - map_bump, bump
 * displacement_texname        - disp
 * alpha_texname               - map_d
 */
typedef struct s_tinyobj_material {
  char *name;  //< Name

  /**
   * Color coefficients (reflectivity)
   **/
  tinyobj_coefficient_t ambient;
  tinyobj_coefficient_t diffuse;
  tinyobj_coefficient_t specular;
  tinyobj_coefficient_t transmittance;
  tinyobj_coefficient_t emission;

  float shininess;  //< Specular exponent
  float ior;        //< Index Of Refraction (ior) 'optical density'
  float dissolve;  //< Non-transparency to be alpha 'dissolve' (0.0f transparent -> 1.0f opaque)
  int illum;       //< Illumination model (0 - 10)

  char *ambient_texname;            /* map_Ka */
  char *diffuse_texname;            /* map_Kd */
  char *specular_texname;           /* map_Ks */
  char *specular_highlight_texname; /* map_Ns */
  char *bump_texname;               /* map_bump, bump */
  char *displacement_texname;       /* disp */
  char *alpha_texname;              /* map_d */
} tinyobj_material_t;

/* Structure: tinyobj_shape_t
 * Shapes contained in a given attribute
 *
 * Fields:
 *
 * name         - Group name or object name
 * face_offset  - Face offset of this shape (starting point)
 * length       - Length of this shape (ending point)
 */
typedef struct s_tinyobj_shape {
  char *name;
  size_t face_offset;
  size_t length;
} tinyobj_shape_t;

/* Structure: tinyobj_attrib_t
 * Object attributes
 * <.obj format: http://paulbourke.net/dataformats/obj/>
 *
 * Fields:
 *
 * v                       - Geometric vertices
 * v_count                 - Geometric vertex count
 * vn                      - Vertex normals
 * vn_count                - Vertex normal count
 * vt                      - Texture vertices
 * vt_count                - Texture vertex count
 * vp                      - Parameter space vertices
 * vp_count                - Parameter space vertex count
 * f                       - Object faces
 * f_count                 - Face count
 * triangle_count_total    - Total count of triangles
 * l                       - Lines
 * l_count                 - Line count
 * p                       - Points
 */
typedef struct s_tinyobj_attrib {
  tinyobj_vertex_t *v;           //< Geometric vertices
  size_t v_count;                //< Geometric vertex count
  tinyobj_vertex_normal_t *vn;   //< Vertex normals
  size_t vn_count;               //< Vertex normal count
  tinyobj_vertex_texture_t *vt;  //< Texture vertices
  size_t vt_count;               //< Texture vertex count
  tinyobj_vertex_param_t *vp;    //< Parameter space vertices
  size_t vp_count;               //< Parameter space vertex count

  tinyobj_face_t *f;            //< Object faces
  size_t f_count;               //< Face count
  size_t triangle_count_total;  //< Total count of triangles

  tinyobj_line_t *l;  //< Lines
  size_t l_count;     //< Line count
  tinyobj_point_t p;  //< Points
} tinyobj_attrib_t;

#ifdef TINYOBJ_ENABLE_OLDER_ATTRIBUTE

#ifdef _MSC_VER
#pragma message( \
    "warning: Using deprecated attribute type in TINYOBJ (TINYOBJ_ENABLE_OLDER_ATTRIBUTE)")
#else
#warning \
    "Using deprecated attribute type in TINYOBJ (TINYOBJ_ENABLE_OLDER_ATTRIBUTE)"
#endif
/* Structure: COMPATtinyobj_attrib_t
 * Object attributes *(deprecated)*
 *
 * Fields:
 *
 * vertices           - Array of geometric vertices (x0, y0, z0, x1, y1, z1, ...)
 * num_vertices       - Number of vertices in 'vertices' (the actual array length is num_vertices*3)
 * normals            - Array of vertex normals (i0, j0, k0, i1, j1, k1, ...)
 * num_normals        - Number of vertices in 'normals' (the actual array length is num_normals*3)
 * texcoords          - Array of  texture vertices (u0, w0, u1, w1, ...)
 * num_texcoords      - Number of vertices in 'texcoords' (the actual array length is num_normals*2)
 * faces              - Array of faces (containing <tinyobj_vertex_index_t> information)
 * num_faces          - Length of 'faces'
 * face_num_verts     - Array of number of vertices in each face (3 for triangulated faces)
 * num_face_num_verts - Total number of triangles in this object (length of face_num_verts)
 * material_ids       - Array with the id of the material for each face (length is num_faces)
 **/
typedef struct {
  unsigned int num_vertices;
  unsigned int num_normals;
  unsigned int num_texcoords;
  unsigned int num_faces;
  unsigned int num_face_num_verts;

  int pad0;

  float *vertices;
  float *normals;
  float *texcoords;
  tinyobj_vertex_index_t *faces;
  int *face_num_verts;
  int *material_ids;
} COMPATtinyobj_attrib_t;

int tinyobj_new2old(tinyobj_attrib_t *attrib,
                    COMPATtinyobj_attrib_t *out_attrib);
void tinyobj_attrib_free_compat(COMPATtinyobj_attrib_t *attrib);
#endif  // TINYOBJ_ENABLE_OLDER_ATTRIBUTE

int tinyobj_parse_mtl_file(tinyobj_material_t **materials_out,
                           unsigned int *num_materials_out,
                           const char *filename);
void tinyobj_material_free(tinyobj_material_t *materials,
                           unsigned int num_materials);
void tinyobj_shape_free(tinyobj_shape_t *shapes, unsigned int num_shapes);
void tinyobj_attrib_free(tinyobj_attrib_t *attrib);
int tinyobj_parse_obj(tinyobj_attrib_t *attrib, tinyobj_shape_t **shapes,
                      unsigned int *num_shapes,
                      tinyobj_material_t **materials_out,
                      unsigned int *num_materials_out, const char *buf,
                      size_t len, unsigned int flags);

#ifdef TINYOBJ_LOADER_C_IMPLEMENTATION
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>  // atoi
#include <string.h>

#if defined(TINYOBJ_MALLOC) && defined(TINYOBJ_REALLOC) && \
    defined(TINYOBJ_CALLOC) && defined(TINYOBJ_FREE)
/* ok */
#elif !defined(TINYOBJ_MALLOC) && !defined(TINYOBJ_REALLOC) && \
    !defined(TINYOBJ_CALLOC) && !defined(TINYOBJ_FREE)
/* ok */
#else
#error \
    "Must define all or none of TINYOBJ_MALLOC, TINYOBJ_REALLOC, TINYOBJ_CALLOC and TINYOBJ_FREE."
#endif

#ifndef TINYOBJ_MALLOC
#define TINYOBJ_MALLOC malloc
#define TINYOBJ_REALLOC realloc
#define TINYOBJ_CALLOC calloc
#define TINYOBJ_FREE free
#endif

#ifdef TINYOBJ_USE_UTHASH
#include <uthash.h>
#endif

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

/***********************************************************************************************
 * Group: String handling
 ***********************************************************************************************/

/* Functions: String handling macros
 *
 * IS_SPACE     - Is this character a spacing character ' ' or '\t'
 * IS_DIGIT     - Is this character a digit
 * IS_NEW_LINE  - Is this character a new line character '\r','\n','\0'
 */
#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) ((unsigned int)((x) - '0') < (unsigned int)(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

/* Function: is_line_ending
 * Is given line ending in this character?
 *
 * Returns:
 *  True if line is ending
 **/
static int is_line_ending(const char *p, size_t i, size_t end_i) {
  if (p[i] == '\0') return 1;
  if (p[i] == '\n') return 1;  // this includes \r\n
  if (p[i] == '\r') {
    if (((i + 1) < end_i) && (p[i + 1] != '\n')) {  // detect only \r case
      return 1;
    }
  }
  return 0;
}

/* Function: skip_space
 * Skips the next spacing characters in given token
 */
static void skip_space(const char **token) {
  while ((*token)[0] == ' ' || (*token)[0] == '\t') {
    if ((*token)[0] == '\0') return;
    (*token)++;
  }
}

/* Function: skip_space_and_cr
 * Skips the next spacing characters and also carriage return in given token
 */
static void skip_space_and_cr(const char **token) {
  while ((*token)[0] == ' ' || (*token)[0] == '\t' || (*token)[0] == '\r') {
    if ((*token)[0] == '\0') return;
    (*token)++;
  }
}

/* Function: length_until_space
 * Calculates and returns the length until next space character in this token
 *
 * Parameters:
 *
 *  token  - Token to be used
 *  n      - Maximum number of characters to count
 *
 * Returns:
 *
 *  Length until next space character
 */
static size_t length_until_space(const char *token, size_t n) {
  size_t len = 0;
  for (len = 0; len < n - 1; len++) {
    if (token[len] == '\n' || token[len] == ' ' || token[len] == '\t' ||
        token[len] == '\r')
      break;
  }
  return len;
}

/* Function: until_space
 * Skips the next spacing characters
 * > '\0',' ', '\t', '\r'
 */
static int until_space(const char *token) {
  const char *p = token;
  while (p[0] != '\0' && p[0] != ' ' && p[0] != '\t' && p[0] != '\r') {
    p++;
  }

  return (int)(p - token);
}

/* Function: until_space_cr_slash
 * Skips the next characters in given token
 * > '\0','/', ' ', '\t', '\r'
 */
static void until_space_cr_slash(const char **token) {
  while ((*token)[0] != '\0' && (*token)[0] != '/' && (*token)[0] != ' ' &&
         (*token)[0] != '\t' && (*token)[0] != '\r')
    (*token)++;
}

/* Function: length_until_newline_comment_space
 *
 * Returns the length until the next characters:
 * > \n \r\n \0 ' ' \t #
 *
 * Parameters:
 * token  - Buffer
 * n      - Maximum number of characters to count
 *
 * Returns:
 *
 *  Length until next character
 */
static size_t length_until_newline_comment_space(const char *token, size_t n) {
	size_t len = 0;
	for(len = 0; len < n-1; len++) {
		if(token[len] == '\n')
			break;
		if((token[len] == '\r') && ((len < (n - 2)) && (token[len + 1] != '\n')))
			break;
		if(token[len] == '\0' || token[len] == ' ' || token[len] == '\t' || token[len] == '#')
			break;
	}
	return len;
}

/* Function: length_until_line_feed
 *
 * Returns the length until the next line feed
 *
 * *Assumes token[n-1] = '\0'*
 *
 * Parameters:
 * token  - Buffer
 * n      - Maximum number of characters to count
 */
static size_t length_until_line_feed(const char *token, size_t n) {
  size_t len = 0;

  for (len = 0; len < n; len++) {
    if ((token[len] == '\n') || (token[len] == '\r')) break;
  }
  return len;
}

/* Function: strdup_ml
 *  Duplicates 'max_length' characters of given string
 *  (strdup with max length specified)
 *
 * Parameters:
 *
 *  s           - String to be duplicated
 *  max_length  - Maximum length to be duplicated
 *
 * Returns:
 *
 *   Duplicated string (*should be freed by the caller*)
 */
static char *strdup_ml(const char *s, size_t max_length) {
  char *d;
  size_t len;

  if (s == NULL) return NULL;

  /* Do not consider CRLF line ending(#19) */
  len = length_until_line_feed(s, max_length);
  /* len = strlen(s); */

  /* trim line ending and append '\0' */
  d = TINYOBJ_MALLOC(len + 1); /* + '\0' */
  memcpy(d, s, len);
  d[len] = '\0';

  return d;
}

#ifndef strndup
/* Function: strndup
 * strndup implementation for non-GNU compliant compilers
 */
char *strndup(const char *s, size_t len) {
  char *d;
  size_t slen;

  if (s == NULL || len == 0) return NULL;

  d = TINYOBJ_MALLOC(len + 1); /* + '\0' */
  if (d == NULL) return NULL;

  slen = strlen(s);
  if (slen < len) {
    memcpy(d, s, slen);
    d[slen] = '\0';
  } else {
    memcpy(d, s, len);
    d[len] = '\0';
  }

  return d;
}
#endif

/* Function: dynamic_fgets
 * fgets with a dynamic buffer
 */
static char *dynamic_fgets(char **buf, int *size, FILE *file) {
  char *offset;
  char *ret;
  int old_size;

  if (!(ret = fgets(*buf, (int)*size, file))) {
    return ret;
  }

  if (NULL != strchr(*buf, '\n')) {
    return ret;
  }

  do {
    old_size = *size;
    *size *= 2;
    *buf = TINYOBJ_REALLOC(*buf, *size);
    offset = &((*buf)[old_size - 1]);

    ret = fgets(offset, (int)(old_size + 1), file);
  } while (ret && (NULL == strchr(*buf, '\n')));

  return ret;
}

/***********************************************************************************************
 * Group: Triplet handling
 ***********************************************************************************************/

/* Function: fixIndex
 *
 * Converts given absolute index, from triplet to zero-base, supports relative
 * indices
 *
 * See:
 *
 * - <tinyobj_vertex_index_t>
 * - <parseRawTriple>
 * - <tinyobj_attrib_construct>
 *
 *
 * Parameters:
 *
 *  idx  - Index to be converted
 *  n    - Position of this couple/tuple regarding to the vertex type of this index
 *
 * Returns:
 *
 *  - Fixed index value that's able to be used when accessing vertex arrays
 *  - <TINYOBJ_INVALID_INDEX> when there's no equivalent index
 */
static int fixIndex(int idx, size_t n) {
  if (idx > 0) return idx - 1;
  if (idx == 0) return 0;
  if (idx == TINYOBJ_INVALID_INDEX) return TINYOBJ_INVALID_INDEX;

  return (int)n + idx;  // Negative value = relative
}

/* Function: parseRawTriple
 *
 * Parses raw triplets of given tokens
 * > v, v/vt/vn, v//vn, v/vt
 *
 * > Valid triplets:
 * >  <x> 1//1
 * >  <x> 1//1 2//2 3//3 4//4
 * >  <x> 1/1/1 2/2/2 3/3/3 4/4/4
 * > The following are examples of illegal statements
 * >  <x> 1/1/1 2/2/2 3//3 4//4
 * >  <x> 1/ 1/1 2/2/2 3/3/3 4/4/4
 *
 * Parameters:
 *  token - Token to parse
 *
 * Returns:
 *  Indices parsed from given token, if one of the indices is not present it's
 *  equal to <TINYOBJ_INVALID_INDEX>
 */
static tinyobj_vertex_index_t parseRawTriple(const char **token) {
  tinyobj_vertex_index_t vi;

  vi.v_idx = (int)(TINYOBJ_INVALID_INDEX);
  vi.vn_idx = (int)(TINYOBJ_INVALID_INDEX);
  vi.vt_idx = (int)(TINYOBJ_INVALID_INDEX);

  // v
  vi.v_idx = atoi((*token));
  until_space_cr_slash(token);
  if ((*token)[0] != '/') return vi;
  (*token)++;

  // v//vn
  if ((*token)[0] == '/') {
    (*token)++;
    vi.vn_idx = atoi((*token));
    until_space_cr_slash(token);
    return vi;
  }

  // v/vt/vn or v/vt
  vi.vt_idx = atoi((*token));
  until_space_cr_slash(token);
  if ((*token)[0] != '/') return vi;

  // v/vt/vn
  (*token)++;  // skip '/'
  vi.vn_idx = atoi((*token));
  until_space_cr_slash(token);

  return vi;
}

/***********************************************************************************************
 * Group: Integer/Double/Float handling
 ***********************************************************************************************/

/* Function: parseInt
 * Returns equivalent integer of next non-space character, the token is advanced
 * until next space
 */
static int parseInt(const char **token) {
  int i = 0;
  skip_space(token);
  i = atoi((*token));
  (*token) += until_space((*token));
  return i;
}

/* Function: tryParseDouble_assemble
 *
 * Last step of <tryParseDouble>
 *
 * Assembles given information into double
 *
 * Parameters:
 *  sign     - Sign of this double
 *  mantissa - Mantissa
 *  exp_sign - Exponent sign
 *  exponent - Exponent
 *
 * Returns:
 *
 *  Assembled double
 */
static double tryParseDouble_assemble(char sign, double mantissa, char exp_sign,
                                      int exponent) {
  double a = 1.0; /* = pow(5.0, exponent); */
  double b = 1.0; /* = 2.0^exponent */
  int i;

  for (i = 0; i < exponent; i++) a = a * 5.0;

  for (i = 0; i < exponent; i++) b = b * 2.0;

  if (exp_sign == '-') {
    a = 1.0 / a;
    b = 1.0 / b;
  }

  /* (sign == '+' ? 1 : -1) * ldexp(mantissa * pow(5.0, exponent),
     exponent); */
  return (sign == '+' ? 1 : -1) * (mantissa * a * b);
}

/* Function: tryParseDouble_integer
 *
 * Part of <tryParseDouble>
 *
 * Parses integer part of a double (advances curr)
 *
 * Parameters:
 *
 * curr     - Current position in a buffer
 * s_end    - End of the buffer
 * sign     - [out] Sign to be filled
 * integer  - [out] Integer to be filled
 *
 * Returns:
 *   True integer was successfuly parsed, false if nothing was read
 */
static unsigned char tryParseDouble_integer(const char **curr,
                                            const char *s_end, char *sign,
                                            int *integer) {
  unsigned int read = 0;
  *sign = '+';  // Default sign

  // Read sign
  if ((*curr)[0] == '+' || (*curr)[0] == '-') {
    (*sign) = (*curr)[0];
    if ((*curr)++ == s_end) return 0;  // No more data to read (invalid number)
  } else if (!IS_DIGIT((*curr)[0])) {
    return 0;  // A digit is expected
  }

  // Read integer
  for (; (*curr) != s_end && IS_DIGIT(((*curr)[0])); (*curr)++, read++) {
    (*integer) *= 10;
    (*integer) += (int)((*curr)[0] - 0x30);
  }
  return (read > 0);
}

/* Function: tryParseDouble
 *
 * Tries to parse a floating point number located at s.
 * > Parses the following EBNF grammar:
 * >   sign    = "+" | "-" ;
 * >   END     = ? anything not in digit ?
 * >   digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
 * >   integer = [sign] , digit , {digit} ;
 * >   decimal = integer , ["." , integer] ;
 * >   float   = ( decimal , END ) | ( decimal , ("E" | "e") , integer , END ) ;
 *
 * > Valid strings are for example:
 * >  -0  +3.1417e+2  -0.0E-3  1.0324  -1.41   11e2
 *
 * The function is greedy and will parse until any of the following happens:
 *
 *  - a non-conforming character is encountered.
 *  - s_end is reached.
 *
 * The following situations triggers a failure:
 *
 *  - s >= s_end.
 *  - parse failure.
 *
 * Parameters:
 *
 *  s      - String to be parsed
 *  s_end  - Location in the string where reading should halt, e.g. the end of
 * the string result - [out] Parsed double
 *
 * Returns
 *  If the parsing is a success, result is set to the parsed value and true
 *  is returned.
 */
static unsigned char tryParseDouble(const char *s, const char *s_end,
                                    double *result) {
  double mantissa = 0.0;
  int integer = 0;
  /* This exponent is base 2 rather than 10.
   * However the exponent we parse is supposed to be one of ten,
   * thus we must take care to convert the exponent/and or the
   * mantissa to a * 2^E, where a is the mantissa and E is the
   * exponent.
   * To get the final double we will use ldexp, it requires the
   * exponent to be in base 2.
   */
  int exponent = 0;

  /* NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
   * TO JUMP OVER DEFINITIONS.
   */
  char sign = '+';
  char exp_sign = '+';
  char const *curr = s;

  /* How many characters were read in a loop. */
  int read = 0;

  /* BEGIN PARSING. */

  if (s >= s_end)  // Passed stopping point
    return 0;

  // Read integer and sign
  if (!tryParseDouble_integer(&curr, s_end, &sign, &integer))
    return 0;  // Nothing was read
  mantissa = integer;

  while (curr != s_end) {
    switch (*curr) {
      // Read decimal
      case '.': {
        if (curr++ == s_end) return 0;
        read = 1;
        while (curr != s_end && IS_DIGIT(*curr)) {
          /* pow(10.0, -read) */
          double frac_value = 1.0;
          int f;
          for (f = 0; f < read; f++) frac_value *= 0.1;
          mantissa += (int)(*curr - 0x30) * frac_value;
          read++;
          curr++;
        }
        break;
      }
      // Read Exponent
      case 'E':
      case 'e': {
        if (curr++ == s_end) return 0;
        if (!tryParseDouble_integer(&curr, s_end, &exp_sign, &exponent))
          return 0;  // Empty E is not allowed
        break;
      }
      default:
        curr++;
        break;
    }
  }
  *result = tryParseDouble_assemble(sign, mantissa, exp_sign, exponent);
  return 1;
}

/* Function: try_parse_float
 *
 * Tries to parse next float, if there's none fails
 *
 * Parameters:
 *
 *  token   - Buffer to be read
 *  result  - [out] Result, not set if return is false
 *
 * Returns:
 *
 *  If the parsing is a success, result is set to the parsed value and true
 *  is returned.
 **/
static unsigned char try_parse_float(const char **token, float *result) {
  const char *end;
  double val = 0.0;
  float f = 0.0f;
  unsigned char retval;

  skip_space(token);
  end = (*token) + until_space((*token));
  val = 0.0;
  retval = tryParseDouble((*token), end, &val);
  *result = (retval) ? (float)val : f;
  (*token) = end;
  return retval;
}

/* Function: parseFloat
 *
 * Returns equivalent float of next non-space character, the token is advanced
 * until next space
 */
static float parseFloat(const char **token) {
  float f = 0.0f;
  try_parse_float(token, &f);
  return f;
}

/* Function: parseFloat2
 *
 * Parses the two next floats in given token
 *
 * See:
 *
 *  <parseFloat>
 */
static void parseFloat2(float *x, float *y, const char **token) {
  (*x) = parseFloat(token);
  (*y) = parseFloat(token);
}

/* Function: parseFloat3
 *
 * Parses the three next floats in given token
 *
 * See:
 *
 *  <parseFloat>
 */
static void parseFloat3(float *x, float *y, float *z, const char **token) {
  (*x) = parseFloat(token);
  (*y) = parseFloat(token);
  (*z) = parseFloat(token);
}

/***********************************************************************************************
 * Group: TINYOBJ hashtable implementation
 * String to int hashtable
 ***********************************************************************************************/

/* Constants: Hash table return codes
 *
 * TINYOBJ_HASH_TABLE_SUCCESS - Success
 * TINYOBJ_HASH_TABLE_ERROR   - Failure
 */
#define TINYOBJ_HASH_TABLE_SUCCESS (0)
#define TINYOBJ_HASH_TABLE_ERROR (-1)

/* Constant: TINYOBJ_HASH_TABLE_DEFAULT_SIZE
 * Default starting size for tinyobj own hash table implementation tables
 */
#define TINYOBJ_HASH_TABLE_DEFAULT_SIZE (10)

#ifndef TINYOBJ_USE_UTHASH
/* Structure: hash_table_entry_t
 *
 * Entries used in tinyobj's own hash table implementation
 *
 * Fields:
 *
 *  hash   - Hash value
 *  filled - Is this entry filled
 *  value  - Value
 *  next   - Next entry
 */
typedef struct s_hash_table_entry_t {
  unsigned long hash;
  int filled;
  int pad0;
  long value;

  struct s_hash_table_entry_t *next;
} hash_table_entry_t;

/* Structure: tinyobj_material_table_t
 *
 * Hash table used in tinyobj's own hash table implementation
 *
 * Fields:
 *
 *  hashes   - Hashes used
 *  entries  - Linked list of entries in this table
 *  capacity - Total capacity of this table
 *  n        - Count of elements
 */
typedef struct {
  unsigned long *hashes;
  hash_table_entry_t *entries;
  size_t capacity;
  size_t n;
} tinyobj_material_table_t;

// Group: TINYOBJ hashtable implementation
// String to int hashtable

/* Function: hash_djb2
 * Converts given string to hash
 */
static unsigned long hash_djb2(const unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + (unsigned long)(c);
  }

  return hash;
}

/* Function: create_hash_table
 *
 * Creates a new hash table in 'hash_table'
 *
 * Parameters:
 *
 *  start_capacity - Starting capacity, if equal to zero is <TINYOBJ_HASH_TABLE_DEFAULT_SIZE>
 *  hash_table     - [in/out] Table to be initialized
 */
static void create_hash_table(size_t start_capacity,
                              tinyobj_material_table_t *hash_table) {
  if (start_capacity < 1) start_capacity = TINYOBJ_HASH_TABLE_DEFAULT_SIZE;
  hash_table->hashes =
      (unsigned long *)TINYOBJ_MALLOC(start_capacity * sizeof(unsigned long));
  hash_table->entries = (hash_table_entry_t *)TINYOBJ_CALLOC(
      start_capacity, sizeof(hash_table_entry_t));
  hash_table->capacity = start_capacity;
  hash_table->n = 0;
}

/* Function: destroy_hash_table
 * Finalizes given hash_table
 */
static void destroy_hash_table(tinyobj_material_table_t *hash_table) {
  if (!hash_table) return;
  if (hash_table->entries) TINYOBJ_FREE(hash_table->entries);
  if (hash_table->hashes) TINYOBJ_FREE(hash_table->hashes);
}

/* Function: hash_table_insert_value
 *
 *  Subroutine of <hash_table_insert>
 *  Inserts given value in hash, uses quadratic probing
 *
 * Parameters:
 *
 *  hash       - Hash to be used
 *  value      - Value of this entity
 *  hash_table - Table to be used
 *
 * Returns:
 *  <TINYOBJ_HASH_TABLE_ERROR> upon failure or <TINYOBJ_HASH_TABLE_SUCCESS> after succeding
 */
static int hash_table_insert_value(unsigned long hash, long value,
                                   tinyobj_material_table_t *hash_table) {
  /* Insert value */
  size_t start_index = hash % hash_table->capacity;
  size_t index = start_index;
  hash_table_entry_t *start_entry = hash_table->entries + start_index;
  size_t i;
  hash_table_entry_t *entry;

  for (i = 1; hash_table->entries[index].filled; i++) {
    if (i >= hash_table->capacity) return TINYOBJ_HASH_TABLE_ERROR;
    index = (start_index + (i * i)) % hash_table->capacity;
  }

  entry = hash_table->entries + index;
  entry->hash = hash;
  entry->filled = 1;
  entry->value = value;

  if (index != start_index) {
    /* This is a new entry, but not the start entry, hence we need to add a next
     * pointer to our entry */
    entry->next = start_entry->next;
    start_entry->next = entry;
  }

  return TINYOBJ_HASH_TABLE_SUCCESS;
}

/* Function: hash_table_insert
 *
 *  Inserts given value in hash, calls <hash_table_insert_value>
 *
 * Parameters:
 *
 *  hash       - Hash to be used
 *  value      - Value of this entity
 *  hash_table - Table to be used
 *
 * Returns:
 *  <TINYOBJ_HASH_TABLE_ERROR> upon failure or <TINYOBJ_HASH_TABLE_SUCCESS> after succeding
 */
static int hash_table_insert(unsigned long hash, long value,
                             tinyobj_material_table_t *hash_table) {
  int ret = hash_table_insert_value(hash, value, hash_table);
  if (ret == TINYOBJ_HASH_TABLE_SUCCESS) {
    hash_table->hashes[hash_table->n] = hash;
    hash_table->n++;
  }
  return ret;
}

/* Function: hash_table_find
 *
 *  Tries to find given hash in the table
 *
 * Parameters:
 *
 *  hash       - Hash to be found
 *  hash_table - Table to be used
 *
 * Returns:
 *
 *  Entry upon succeding or NULL if failure to find
 */
static hash_table_entry_t *hash_table_find(
    unsigned long hash, tinyobj_material_table_t *hash_table) {
  hash_table_entry_t *entry =
      hash_table->entries + (hash % hash_table->capacity);
  while (entry) {
    if (entry->hash == hash && entry->filled) return entry;
    entry = entry->next;
  }
  return NULL;
}

/* Function: hash_table_maybe_grow
 * Grows given hash_table to new_n if new_n is less or equal to capacity
 */
static void hash_table_maybe_grow(size_t new_n,
                                  tinyobj_material_table_t *hash_table) {
  size_t new_capacity;
  tinyobj_material_table_t new_hash_table;
  size_t i;

  if (new_n <= hash_table->capacity) return;
  new_capacity =
      2 * ((2 * hash_table->capacity) > new_n ? hash_table->capacity : new_n);
  /* Create a new hash table. We're not calling create_hash_table because we
   * want to realloc the hash array */
  new_hash_table.hashes = hash_table->hashes = (unsigned long *)TINYOBJ_REALLOC(
      (void *)hash_table->hashes, sizeof(unsigned long) * new_capacity);
  new_hash_table.entries = (hash_table_entry_t *)TINYOBJ_CALLOC(
      new_capacity, sizeof(hash_table_entry_t));
  new_hash_table.capacity = new_capacity;
  new_hash_table.n = hash_table->n;

  /* Rehash */
  for (i = 0; i < hash_table->capacity; i++) {
    hash_table_entry_t *entry =
        hash_table_find(hash_table->hashes[i], hash_table);
    hash_table_insert_value(hash_table->hashes[i], entry->value,
                            &new_hash_table);
  }

  TINYOBJ_FREE(hash_table->entries);
  (*hash_table) = new_hash_table;
}

/* Function: hash_table_exists
 *  Tries to find 'name' in hash_table, returns true if successful
 *
 * See:
 *
 * - <hash_table_find>
 * - <hash_djb2>
 */
static int hash_table_exists(const char *name,
                             tinyobj_material_table_t *hash_table) {
  return hash_table_find(hash_djb2((const unsigned char *)name), hash_table) !=
         NULL;
}

/* Function: hash_table_set
 *  Inserts value into hash_table, in name; creates new entry if it doesn't
 * exist
 *
 * See:
 *
 * - <hash_djb2>
 * - <hash_table_insert>
 */
static void hash_table_set(const char *name, size_t val,
                           tinyobj_material_table_t *hash_table) {
  /* Hash name */
  unsigned long hash = hash_djb2((const unsigned char *)name);

  hash_table_entry_t *entry = hash_table_find(hash, hash_table);
  if (entry) {
    entry->value = (long)val;
    return;
  }

  /* Expand if necessary
   * Grow until the element has been added
   */
  do {
    hash_table_maybe_grow(hash_table->n + 1, hash_table);
  } while (hash_table_insert(hash, (long)val, hash_table) !=
           TINYOBJ_HASH_TABLE_SUCCESS);
}

/* Function: hash_table_get
 * Returns value of entity with name, -1 when failing
 */
static long hash_table_get(const char *name,
                           tinyobj_material_table_t *hash_table) {
  hash_table_entry_t *ret =
      hash_table_find(hash_djb2((const unsigned char *)(name)), hash_table);
  return (ret) ? ret->value : -1;
}
#else
/***********************************************************************************************
 * Group: UTHash interface to tinyobj
 ***********************************************************************************************/

/* Sturcture: tinyobj_material_table_tEntry
 *
 *  Material table entry information
 *
 * Fields:
 *
 *  name  - Nul terminated name (key)
 *  value - Value of this entity
 *  hh    - UTHash handle
 */
typedef struct s_material_table_entry {
  char *name;  // Nul terminated name (key)
  long value;

  UT_hash_handle hh;
} tinyobj_material_table_tEntry;

/* Structure: tinyobj_material_table_t
 * Material table
 */
typedef struct s_material_table {
  tinyobj_material_table_tEntry *head;
} tinyobj_material_table_t;

// Group: UTHash interface to tinyobj

/* Constant: tinyobj_reserved
 * Reserved key used by the hashtable implementation so the table isn't deleted
 */
const char *tinyobj_reserved = "_RESERVED_KEY_";

/* Function: tinyobj_hash_find
 *
 * Tries to find object with 'name' as key
 *
 * Parameters:
 *
 *  table    - Table to be used
 *  name     - Key
 *  entryOUT - [out] Result
 *
 * Returns:
 *
 *  Sets entry to a valid result if entry was found, otherwise sets it to NULL
 */
static void tinyobj_hash_find(tinyobj_material_table_t *table, const char *name,
                              tinyobj_material_table_tEntry **entryOUT) {
  tinyobj_material_table_tEntry *entry = NULL;
  if (table) HASH_FIND_STR(table->head, name, entry);
  *entryOUT = entry;
}

/* Function: tinyobj_hash_add
 *
 * Adds a material to the given material table
 * If the material was already added its value is replaced
 *
 * Parameters:
 *
 * table   - Table to be used
 * name    - Key
 * length  - Name length
 * value   - Value of the new entity
 *
 * Returns:
 *  Returns true if successful, false otherwise
 */
static int tinyobj_hash_add(tinyobj_material_table_t *table, const char *name,
                            size_t length, long value) {
  tinyobj_material_table_tEntry *entry = NULL;

  if (!table) return TINYOBJ_HASH_TABLE_ERROR;

  HASH_FIND_STR(table->head, name, entry);
  if (entry) {
    entry->value = value;
    return TINYOBJ_HASH_TABLE_SUCCESS;
  }
  entry = TINYOBJ_MALLOC(sizeof(*entry));
  if (!entry) return TINYOBJ_HASH_TABLE_ERROR;

  entry->name = name;
  entry->value = value;

  HASH_ADD_KEYPTR(hh, table->head, name, length, entry);
  return TINYOBJ_HASH_TABLE_SUCCESS;
}

/* Function: tinyobj_hash_free
 *
 *  Frees all entries of a given tinyobj_material_table_t
 *
 * Parameters:
 *
 *  table      - Table to be freed
 *  bfree_name - Should the key (name) be freed
 *
 *               Usually the name pointer is owned by the materials list by this point
 *
 *               See <tinyobj_parse_and_index_mtl_file>, freeing could lead to a segfault
 */
static void tinyobj_hash_free(tinyobj_material_table_t *table,
                              unsigned char bfree_name) {
  tinyobj_material_table_tEntry *entry, *tmp;
  if (!table) return;

  HASH_ITER(hh, table->head, entry, tmp) {
    HASH_DEL(table->head, entry);
    if (entry->name && bfree_name && entry->value != -1)
      TINYOBJ_FREE(entry->name);
    TINYOBJ_FREE(entry);
  }
}

/* Function: tinyobj_hash_init
 *  Initialises given table, and adds a reserved key
 */
static void tinyobj_hash_init(tinyobj_material_table_t *table) {
  table->head = NULL;
  tinyobj_hash_add(table, tinyobj_reserved, strlen(tinyobj_reserved), -1);
}

#endif  // TINYOBJ_USE_UTHASH
/***********************************************************************************************
 * Group: Material handling (.mtl)
 ***********************************************************************************************/

/* Function: initMaterial
 *  Defaults given material
 */
static void initMaterial(tinyobj_material_t *material) {
  material->name = NULL;
  material->ambient_texname = NULL;
  material->diffuse_texname = NULL;
  material->specular_texname = NULL;
  material->specular_highlight_texname = NULL;
  material->bump_texname = NULL;
  material->displacement_texname = NULL;
  material->alpha_texname = NULL;
  material->ambient.r = material->ambient.g = material->ambient.b = 0.f;
  material->diffuse.r = material->diffuse.g = material->diffuse.b = 0.f;
  material->specular.r = material->specular.g = material->specular.b = 0.f;
  material->transmittance.r = material->transmittance.g =
      material->transmittance.b = 0.f;
  material->emission.r = material->emission.g = material->emission.b = 0.f;

  material->illum = 0;
  material->dissolve = 1.f;
  material->shininess = 1.f;
  material->ior = 1.f;
}

/* Function: tinyobj_mtl_parse_map
 *
 * Parses a texture name from token ('map_')
 * > Supported map types:
 * >    map_Ka   - ambient texture
 * >    map_Kd   - diffuse texture
 * >    map_Ks   - specular texture
 * >    map_Ns   - specular highlight texture
 * >    map_bump - bump texture
 * >    map_d    - alpha texture
 *
 * Parameters:
 *
 *  material - Material to be filled
 *  token    - Buffer
 *  line_end - Buffer end (halting point)
 *
 * Returns:
 *   - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *   - <TINYOBJ_SUCCESS>
 */
static int tinyobj_mtl_parse_map(tinyobj_material_t *material,
                                 const char **token, const char *line_end) {
  *token += 4;
  switch ((*token)[0]) {
    case 'K': {
      char subtype = (*token)[1];
      if (!IS_SPACE((*token)[2])) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      *token += 3;
      switch (subtype) {
        case 'a':  // ambient texture
          material->ambient_texname = strdup_ml(*token, (line_end - (*token)));
          break;
        case 'd':  // diffuse texture
          material->diffuse_texname = strdup_ml(*token, (line_end - (*token)));
          break;
        case 's':  // specular texture
          material->specular_texname = strdup_ml(*token, (line_end - (*token)));
          break;
        default:
          return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      }
      break;
    }
    case 'N':  // specular highlight texture
      if ((*token)[1] != 's' || !IS_SPACE((*token)[2]))
        return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      *token += 3;
      material->specular_highlight_texname =
          strdup_ml(*token, (line_end - (*token)));
      break;
    case 'b':  // bump texture
      if (strncmp(*token, "bump", 4) && !IS_SPACE((*token)[5]))
        return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      *token += 6;
      material->bump_texname = strdup_ml(*token, (line_end - (*token)));
      break;
    case 'd':  // alpha texture
      if (!IS_SPACE((*token)[1])) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      *token += 2;
      material->alpha_texname = strdup_ml(*token, (line_end - (*token)));
      break;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_mtl_parse_optical
 *
 * Parses an optical coefficient from token ('N')
 * > Supported coefficient types:
 * >   Ni - Index Of Refraction (ior) 'optical density'
 * >   Ns - Specular exponent 'shininess'
 *
 * Parameters:
 *
 *  material - Material to be filled
 *  token    - Buffer
 *
 * Returns:
 *   - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *   - <TINYOBJ_SUCCESS>
 */
static int tinyobj_mtl_parse_optical(tinyobj_material_t *material,
                                     const char **token) {
  char subtype = (*token)[1];

  if (!IS_SPACE(((*token)[2]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;

  *token += 3;
  switch (subtype) {
    case 'i':  // ior (index of refraction)
      material->ior = parseFloat(token);
      break;
    case 's':  // shininess
      material->shininess = parseFloat(token);
      break;
    default:
      return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_mtl_parse_color
 *
 * Parses a color coefficient from token ('K')
 * > Supported coefficient types:
 * >  Ka - Ambient
 * >  Kd - Diffuse
 * >  Ks - Specular
 * >  Kt - Transmittance
 * >  Ke - Emissive
 *
 * Parameters:
 *
 *  material - Material to be filled
 *  token    - Buffer
 *
 * Returns:
 *   - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *   - <TINYOBJ_SUCCESS>
 */
static int tinyobj_mtl_parse_color(tinyobj_material_t *material,
                                   const char **token) {
  float r, g, b;
  char subtype = (*token)[1];

  if (!IS_SPACE(((*token)[2]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;

  *token += 3;
  parseFloat3(&r, &g, &b, token);
  switch (subtype) {
    case 'a':  // ambient
      material->ambient.r = r;
      material->ambient.g = g;
      material->ambient.b = b;
      break;
    case 'd':  // diffuse
      material->diffuse.r = r;
      material->diffuse.g = g;
      material->diffuse.b = b;
      break;
    case 's':  // specular
      material->specular.r = r;
      material->specular.g = g;
      material->specular.b = b;
      break;
    case 't':  // transmittance
      material->transmittance.r = r;
      material->transmittance.g = g;
      material->transmittance.b = b;
      break;
    case 'e':  // emission
      material->emission.r = r;
      material->emission.g = g;
      material->emission.b = b;
      break;
    default:
      return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_parse_and_index_mtl_file
 *
 * Parses and indexes a given material file
 *
 * Parameters:
 *
 *  materials_out     - [out] Completed list of materials
 *  num_materials_out - [out] Length of materials_out
 *  filename          - File to be parsed
 *  material_table    - Table to be filled (can be NULL)
 *
 * Returns:
 *   - <TINYOBJ_ERROR_FILE_OPERATION>
 *   - <TINYOBJ_ERROR_MEMORY>
 *   - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *   - <TINYOBJ_SUCCESS>
 */
static int tinyobj_parse_and_index_mtl_file(
    tinyobj_material_t **materials_out, unsigned int *num_materials_out,
    const char *filename, tinyobj_material_table_t *material_table) {
  int buffer_size = 128;  // dynamic_fgets supports only integers as sizes
  char *linebuf;
  FILE *fp;
  tinyobj_material_t *material = NULL;
  tinyobj_material_t *material_list = NULL;
  size_t material_list_length = 0;
  unsigned int material_count = 0;
  const char *line_end = NULL;

  if (materials_out == NULL || num_materials_out == NULL)
    return TINYOBJ_ERROR_INVALID_PARAMETER;

  (*materials_out) = NULL;
  (*num_materials_out) = 0;

  material_list =
      malloc(sizeof(*material_list) * TINYOBJ_MATERIAL_INITIAL_COUNT);
  material_list_length = TINYOBJ_MATERIAL_INITIAL_COUNT;
  if (!material_list) return TINYOBJ_ERROR_MEMORY;
  material = &material_list[0];
  initMaterial(material);

  fp = fopen(filename, "r");
  if (!fp) {
    free(material_list);
    return TINYOBJ_ERROR_FILE_OPERATION;
  }

  linebuf = TINYOBJ_MALLOC(buffer_size);
  while (NULL != dynamic_fgets(&linebuf, &buffer_size, fp)) {
    const char *token = linebuf;
    int retval = TINYOBJ_ERROR_NOT_SET;
    line_end = token + strlen(token);

    // Skip leading space
    token += strspn(token, " \t");

    assert(token);
    if (token[0] == '\0') continue;  // Empty line
    if (token[0] == '#') continue;   // Comment line

    // New material
    if ((0 == strncmp(token, "newmtl", 6)) && IS_SPACE((token[6]))) {
      char namebuf[4096];
      material_count++;
      // Update current material
      if (material->name) {
        if (material_count == material_list_length) {
          tinyobj_material_t *temp;
          size_t temp_length;
          temp_length = material_list_length * TINYOBJ_MATERIAL_GROWTH_FACTOR;
          temp = TINYOBJ_REALLOC(material_list,
                                 sizeof(*material_list) * temp_length);
          if (!temp) {
            free(material_list);
            return TINYOBJ_ERROR_MEMORY;
          }
          material_list_length = temp_length;
          material_list = temp;
        }
        material = &material_list[material_count];
      }
      initMaterial(material);
      token += 7;
#ifdef _MSC_VER
      sscanf_s(token, "%s", namebuf, (unsigned)_countof(namebuf));
#else
      sscanf(token, "%s", namebuf);
#endif
      material->name = strdup_ml(namebuf, (line_end - token));
      // Add material to the table
      if (material_table) {
#ifndef TINYOBJ_USE_UTHASH
        hash_table_set(material->name, material_count, material_table);
#else
        tinyobj_hash_add(material_table, material->name, strlen(material->name),
                         material_count);
#endif  // TINYOBJ_USE_UTHASH
      }
      continue;
    }

    switch (token[0]) {
      case 'K':  // Color coefficients
        retval = tinyobj_mtl_parse_color(material, &token);
        break;
      case 'N':  // Optical coefficients
        retval = tinyobj_mtl_parse_optical(material, &token);
        break;
      case 'd':  // Non-transparency to be alpha 'dissolve' 'd'
        // 'disp' Displacement texture
        if ((0 == strncmp(token, "disp", 4)) && IS_SPACE(token[4])) {
          token += 5;
          material->displacement_texname = strdup_ml(token, (line_end - token));
          continue;
        }
      // Fall-through
      case 'T': {  // Transparency to be alpha
        float tr;
        unsigned char type;

        if (!(token[1] == 'r' && IS_SPACE(token[2])) &&
            !(token[0] == 'd' && IS_SPACE(token[1]))) {
          retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
          break;
        }
        type = (token[1] == 'r') ? 2 : 1;
        token += type;
        tr = parseFloat(&token);
        if (tr < 0 || tr > 1) {  // Must be in range [0, 1]
          fprintf(stderr,
                  "%s: Error reading file '%s': Invalid transparency parameter "
                  "'%f', expected range [0, 1]\n",
                  __func__, filename, tr);
          return TINYOBJ_ERROR_MALFORMED_PARAMETER;
        }
        // Invert value of Tr
        material->dissolve = (type == 2) ? 1.0f - tr : tr;
        continue;
      }
      case 'm':  // texture maps
        if (strncmp(token, "map_", 4)) {
          retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
          break;
        }
        retval = tinyobj_mtl_parse_map(material, &token, line_end);
        break;
      case 'i':  // illum model
        if (strncmp(token, "illum", 5) || !IS_SPACE(token[5])) {
          retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
          break;
        }
        token += 6;
        material->illum = parseInt(&token);
        continue;
      case 'b':
        if (strncmp(token, "bump", 4) || !IS_SPACE(token[4])) {
          retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
          break;
        }
        token += 5;
        material->bump_texname = strdup_ml(token, (line_end - token));
        continue;
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case '\0':
        continue;
      default:
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
    }
    if (retval == TINYOBJ_ERROR_UNKNOWN_PARAMETER) {
      char *command_name = NULL;
      size_t length;
      length = length_until_space(token, ((line_end - token)) + 1);
      command_name = strndup(token, length);
      fprintf(stderr, "%s: Unsupported command '%s'\n", __func__, command_name);
      TINYOBJ_FREE(command_name);
    }
  }

  (*num_materials_out) = material_count;
  (*materials_out) = material_list;

  if (linebuf) TINYOBJ_FREE(linebuf);

  return TINYOBJ_SUCCESS;
}

// Group: Aplication Programming Interface (.mtl)

/* Function: tinyobj_parse_mtl_file
 * Parses a material file, see <tinyobj_parse_and_index_mtl_file>
 */
int tinyobj_parse_mtl_file(tinyobj_material_t **materials_out,
                           unsigned int *num_materials_out,
                           const char *filename) {
  int retval = tinyobj_parse_and_index_mtl_file(
      materials_out, num_materials_out, filename, NULL);
  return retval;
}

/* Function: tinyobj_material_free
 *
 * Frees material data
 *
 * Parameters:
 *
 *  materials     - Material list to be freed
 *  num_materials - Length of materials
 */
void tinyobj_material_free(tinyobj_material_t *materials,
                           unsigned int num_materials) {
  unsigned int i;
  if (!materials || !num_materials) return;

  for (i = 0; i < num_materials; i++) {
    if (materials[i].name) TINYOBJ_FREE(materials[i].name);
    if (materials[i].ambient_texname)
      TINYOBJ_FREE(materials[i].ambient_texname);
    if (materials[i].diffuse_texname)
      TINYOBJ_FREE(materials[i].diffuse_texname);
    if (materials[i].specular_texname)
      TINYOBJ_FREE(materials[i].specular_texname);
    if (materials[i].specular_highlight_texname)
      TINYOBJ_FREE(materials[i].specular_highlight_texname);
    if (materials[i].bump_texname) TINYOBJ_FREE(materials[i].bump_texname);
    if (materials[i].displacement_texname)
      TINYOBJ_FREE(materials[i].displacement_texname);
    if (materials[i].alpha_texname) TINYOBJ_FREE(materials[i].alpha_texname);
  }
  TINYOBJ_FREE(materials);
}

/***********************************************************************************************
 * Group: Object handling (.obj)
 ***********************************************************************************************/

/* Enum: CommandType
 *
 * Command identifiers used when parsing
 *
 * <tinyobj_parse_obj_line>
 *
 * COMMAND_EMPTY  - No command
 * COMMAND_V      - 'v' Vertex <tinyobj_obj_parse_vertex>
 * COMMAND_VN     - 'vn' Vertex normal <tinyobj_obj_parse_vertex>
 * COMMAND_VT     - 'vt' Texture coordinate <tinyobj_obj_parse_vertex>
 * COMMAND_VP     - 'vp' Parameter space vertex <tinyobj_obj_parse_vertex>
 * COMMAND_F      - 'f' Face  <tinyobj_obj_parse_face>
 * COMMAND_P      - 'p' Point <tinyobj_obj_parse_point>
 * COMMAND_L      - 'l' Line  <tinyobj_obj_parse_line>
 * COMMAND_G      - 'g' Group name
 * COMMAND_O      - 'o' Object name
 * COMMAND_S      - 's' Smoothing group
 * COMMAND_USEMTL - 'usemtl' Use material
 * COMMAND_MTLLIB -  'mtllib' Load material
 */
typedef enum {
  COMMAND_EMPTY,  //< No command
  // @see tinyobj_obj_parse_vertex
  COMMAND_V,   //< 'v' Vertex
  COMMAND_VN,  //< 'vn' Vertex normal
  COMMAND_VT,  //< 'vt' Texture coordinate
  COMMAND_VP,  //< 'vp' Parameter space vertex

  COMMAND_F,  //< 'f' Face  @see tinyobj_obj_parse_face
  COMMAND_P,  //< 'p' Point @see tinyobj_obj_parse_point
  COMMAND_L,  //< 'l' Line  @see tinyobj_obj_parse_line

  COMMAND_G,       //< 'g' Group name
  COMMAND_O,       //< 'o' Object name
  COMMAND_S,       //< 's' Smoothing group
  COMMAND_USEMTL,  //< 'usemtl' Use material
  COMMAND_MTLLIB   //< 'mtllib' Load material
} CommandType;

/* Structure: Command
 *
 * Command information used to parse obj files
 * <tinyobj_parse_obj_line>
 *
 * Fields:
 *
 *  type - Command
 *  info - Specific command information
 */
typedef struct s_tinyobj_command {
  CommandType type;
  /* Structure: Command.u_information
   *
   * Specific command information
   *
   * Fields:
   *
   *  v               - Geometric vertex <COMMAND_V>
   *  vn              - Vertex normal <COMMAND_VN>
   *  vt              - Texture vertex <COMMAND_VT>
   *  vp              - Parameter space vertex <COMMAND_VP>
   *  f               - Face <COMMAND_F>
   *  l               - Line <COMMAND_L>
   *  p               - Point <COMMAND_P>
   *  g               - Group <COMMAND_G>
   *  o               - Object <COMMAND_O>
   *  usemtl          - Use material <COMMAND_USEMTL>
   *  mtllib          - Material lib (file) <COMMAND_MTLLIB>
   *  smoothing_id    - Smoothing group <COMMAND_S>
   *
   *  See:
   *
   *    <CommandType>
   */
  union u_information {
    /**
     * Vertex Data
     **/
    tinyobj_vertex_t v;
    tinyobj_vertex_normal_t vn;
    tinyobj_vertex_texture_t vt;
    tinyobj_vertex_param_t vp;
    /**
     * Element data
     **/
    tinyobj_face_t f;
    tinyobj_line_t l;
    tinyobj_point_t p;
    /* Structure: Command.s_group_information
     *
     * Grouping and Display/Render attributes
     *
     * Fields:
     *
     *   Name - Name of group
     *   len  - Group name length
     */
    struct s_group_information {
      const char *name;  //< Name of group
      size_t len;        //< Group name length
    } g, o, usemtl, mtllib;
    int smoothing_id;  //< 's' Smoothing group
  } info;
} Command;

/* Structure: CommandInformation
 *
 * General information regarding the commands of a given file
 *
 * Fields:
 *
 *  command_list        - Array of commands
 *  mtllib_line_index   - Index of MLLIB in command list (-1 no command)
 *  count               - command_list count
 *  counter             - Counter of command types
 */
typedef struct s_command_information {
  Command *command_list;  //< Array of commands
  int mtllib_line_index;  //< Index of MLLIB in command list (-1 no command)
  size_t count;           //< Command count
  /* Structure: CommandInformation.s_counter
   * Counter of command types
   *
   * Fields:
   *
   *  v        - Count of geometric vertices
   *  vn       - Count of vertex normals
   *  vt       - Count of texture vertices
   *  vp       - Count of parameter space vertices
   *  f        - Count of faces
   *  l        - Count of lines
   *  p        - Count of points
   *  shapes   - Shapes
   */
  struct s_counter {
    size_t v, vn, vt, vp;
    size_t f, l, p;
    size_t shapes;
  } counter;
} CommandInformation;

// Group: Object handling (.obj)

/* Function: tinyobj_obj_free_point
 * Frees allocated data of a given point
 */
static void tinyobj_obj_free_point(tinyobj_point_t *p) {
  if (p) return;
  if (p->v_idx) TINYOBJ_FREE(p->v_idx);
}

/* Function: tinyobj_obj_parse_point
 *
 * Parses a point from token ('p')
 *
 * Parameters:
 *
 *  command - Command information to be filled
 *  token   - Token to be parsed
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_ERROR_MALFORMED_PARAMETER>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_obj_parse_point(Command *command, const char **token) {
  tinyobj_point_t *p;
  if (!IS_SPACE(((*token)[1]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;

  *token += 2;
  skip_space(token);
  if ((*token)[0] == '#' || !IS_DIGIT(*token[0]))
    return TINYOBJ_ERROR_MALFORMED_PARAMETER;

  p = &command->info.p;
  p->count = 0;
  p->length = TINYOBJ_POINT_INITIAL_COUNT;
  p->v_idx = TINYOBJ_MALLOC(sizeof(*p->v_idx) * p->length);
  if (!p->v_idx) return TINYOBJ_ERROR_MEMORY;

  command->type = COMMAND_P;
  while (!IS_NEW_LINE((*token)[0])) {
    if ((*token)[0] == '#') break;
    if (p->count == p->length) {
      int *tmp;
      p->length *= TINYOBJ_POINT_GROWTH_FACTOR;
      tmp = TINYOBJ_REALLOC(p->v_idx, sizeof(*p->v_idx) * p->length);
      if (tmp) {
        p->v_idx = tmp;
        continue;
      }
      TINYOBJ_FREE(p->v_idx);
      return TINYOBJ_ERROR_MEMORY;
    }
    p->v_idx[p->count] = parseInt(token);
    p->count++;
    skip_space(token);
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_triplet_list_grow
 *
 * Grows the given triplet list to a new size
 *  If the growth fails the list is unchanged
 *
 * Parameters:
 *
 *    list    - List to grow
 *    new_len - New length
 *
 * Returns:
 *
 *   True if successful, false otherwise
 */
unsigned char tinyobj_triplet_list_grow(tinyobj_vertex_index_t **list,
                                        size_t new_len) {
  tinyobj_vertex_index_t *new_list;

  new_list = TINYOBJ_REALLOC(*list, new_len * sizeof(**list));
  if (!new_list) return 0;
  *list = new_list;
  return 1;
}

/* Function: tinyobj_triplet_list_new
 *
 *  Allocates memory for a new triplet list
 *
 * Parameters:
 *
 *   len - Initial length of list
 *
 * Returns:
 *
 *   Pointer to new list when successful, NULL if failure
 */
static tinyobj_vertex_index_t *tinyobj_triplet_list_new(size_t len) {
  return TINYOBJ_MALLOC(sizeof(tinyobj_vertex_index_t) * len);
}

/* Function: tinyobj_obj_free_line
 * Frees allocated data of a given line
 */
static void tinyobj_obj_free_line(tinyobj_line_t *l) {
  if (l) return;
  if (l->couple_list) TINYOBJ_FREE(l->couple_list);
}

/* Function: tinyuobj_obj_parse_line
 *
 * Parses a line from token ('l')
 *
 * Parameters:
 *
 *  command - Command information to be filled
 *  token   - Token to be parsed
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_ERROR_MALFORMED_PARAMETER>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_obj_parse_line(Command *command, const char **token) {
  tinyobj_line_t *l;
  if (!IS_SPACE(((*token)[1]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;

  l = &command->info.l;
  l->count = 0;
  l->length = TINYOBJ_COUPLE_INITIAL_COUNT;
  l->couple_list = TINYOBJ_MALLOC(sizeof(*l->couple_list) * l->length);
  if (!l->couple_list) return TINYOBJ_ERROR_MEMORY;

  command->type = COMMAND_L;

  *token += 2;
  skip_space(token);

  while (!IS_NEW_LINE((*token)[0])) {
    tinyobj_vertex_index_t vi = parseRawTriple(token);
    skip_space_and_cr(token);

    if (vi.vn_idx != TINYOBJ_INVALID_INDEX) {  // This is a triple not a couple
      TINYOBJ_FREE(l->couple_list);
      return TINYOBJ_ERROR_MALFORMED_PARAMETER;
    }
    l->couple_list->v_idx = vi.v_idx;
    l->couple_list->vt_idx = vi.vt_idx;
    l->count++;
    if (l->count == l->length) {
      tinyobj_line_t *tmp;
      l->length *= TINYOBJ_TRIPLET_GROWTH_FACTOR;
      tmp =
          TINYOBJ_REALLOC(l->couple_list, sizeof(*l->couple_list) * l->length);
      if (tmp) {
        l->couple_list = tmp;
        continue;
      }
      TINYOBJ_FREE(l->couple_list);
      return TINYOBJ_ERROR_MALFORMED_PARAMETER;
    }
  }

  if (l->count < 2) {
    // A line must have at least 2 geometric vertices
    fprintf(stderr,
            "%s: malformed line with %ld geometric vertices (minimum of l)\n",
            __func__, l->count);
    TINYOBJ_FREE(l->couple_list);
    return TINYOBJ_ERROR_MALFORMED_PARAMETER;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_obj_free_face
 * Frees allocated data of a given face
 */
static void tinyobj_obj_free_face(tinyobj_face_t *f) {
  if (!f) return;
  if (f->triplet_list) TINYOBJ_FREE(f->triplet_list);
}

/* Function: tinyobj_obj_parse_face
 *
 * Parses a face from token ('f') and fills given command with face information
 *
 * Parameters:
 *
 *  command     - Command information to be filled
 *  token       - Token to be parsed
 *  triangulate - (bool) Should this face be triangulated? <TINYOBJ_FLAG_TRIANGULATE>
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_ERROR_MALFORMED_PARAMETER>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_obj_parse_face(Command *command, const char **token,
                                  int triangulate) {
  tinyobj_face_t *f;
  tinyobj_vertex_index_t *temp_triplet_list;
  unsigned int triplet_count, triplet_length, triangle_count;
  unsigned char last_corner =
      0;  // Is the next vertex index the last corner of a face?

  if (!IS_SPACE(((*token)[1]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;

  f = &command->info.f;

  temp_triplet_list = tinyobj_triplet_list_new(TINYOBJ_TRIPLET_INITIAL_COUNT);
  triplet_length = TINYOBJ_TRIPLET_INITIAL_COUNT;
  triplet_count = 0;
  triangle_count = 0;
  if (!temp_triplet_list) return TINYOBJ_ERROR_MEMORY;

  command->type = COMMAND_F;

  *token += 2;
  skip_space(token);

  if ((*token)[0] == '#') {  // Unexpected comment
    TINYOBJ_FREE(temp_triplet_list);
    return TINYOBJ_ERROR_MALFORMED_PARAMETER;
  }

  while (!IS_NEW_LINE((*token)[0])) {
    tinyobj_vertex_index_t vi = parseRawTriple(token);
    skip_space_and_cr(token);
    if ((*token)[0] == '#') break;

    /**
     * When triangulating we parse the vertices as if they were part of
     * a triangle fan, so every three indices the next is always the first
     * triplet of this face
     **/
    if (triangulate && !last_corner && triplet_count > 2) {
      if (triplet_count == 3) triangle_count++;  // Count first triangle
      temp_triplet_list[triplet_count] = temp_triplet_list[0];
      temp_triplet_list[triplet_count + 1] =
          temp_triplet_list[triplet_count - 1];
      temp_triplet_list[triplet_count + 2] = vi;
      triplet_count += 3;
      triangle_count++;
      last_corner = 1;
    } else {
      temp_triplet_list[triplet_count] = vi;
      triplet_count++;
      last_corner = 0;
    }

    if (triplet_count == triplet_length) {
      triplet_length *= TINYOBJ_TRIPLET_GROWTH_FACTOR;
      if (tinyobj_triplet_list_grow(&temp_triplet_list, triplet_length))
        continue;
      TINYOBJ_FREE(temp_triplet_list);
      return TINYOBJ_ERROR_MEMORY;
    }
  }
  if (f->count < 3) {
    // A face must have at least 3 geometric vertices
    fprintf(stderr,
            "%s: malformed face with %ld geometric vertices (minimum of 3)\n",
            __func__, f->count);
    TINYOBJ_FREE(temp_triplet_list);
    return TINYOBJ_ERROR_MALFORMED_PARAMETER;
  }

  f->triplet_list = temp_triplet_list;
  f->count = triplet_count;
  f->length = triplet_length;
  f->triangle_count = (triangulate) ? triangle_count : 1;
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_obj_parse_vertex
 *
 *  Parses a vertex from token ('v')
 *  > v, vn, vt, vp
 *
 * Parameters:
 *
 *  command     - Command information to be filled
 *  token       - Token to be parsed
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_ERROR_MALFORMED_PARAMETER>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_obj_parse_vertex(Command *command, const char **token) {
  float tmp;
  unsigned char retval;
  char subtype = (*token)[1];
  char type = (IS_SPACE(subtype)) ? 2 : 3;

  if (!IS_SPACE(((*token)[type - 1]))) return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
  *token += type;

  switch (subtype) {
    case ' ':
    case '\t':  // Geometric vertex
      command->type = COMMAND_V;
      parseFloat3(&command->info.v.x, &command->info.v.y, &command->info.v.z,
                  token);
      command->info.v.weight = (try_parse_float(token, &tmp)) ? tmp : 1.0f;
      break;
    case 'n':  // Vertex normal
      command->type = COMMAND_VN;
      parseFloat3(&command->info.vn.i, &command->info.vn.j, &command->info.vn.k,
                  token);
      break;
    case 't':  // Texture vertex
      command->type = COMMAND_VT;
      command->info.vt.u = parseFloat(token);
      retval = try_parse_float(token, &tmp);
      command->info.vt.v = (retval) ? tmp : 0.0f;
      if (retval)
        command->info.vt.w = (try_parse_float(token, &tmp)) ? tmp : 0.0f;
      break;
    case 'p':  // Parameter space vertex
      command->type = COMMAND_VP;
      command->info.vp.u = parseFloat(token);
      retval = try_parse_float(token, &tmp);
      command->info.vp.v = (retval) ? tmp : 0.0f;
      if (retval)
        command->info.vp.weight = (try_parse_float(token, &tmp)) ? tmp : 0.0f;
      break;
    default:
      return TINYOBJ_ERROR_UNKNOWN_PARAMETER;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_parse_obj_line
 *
 * Parses line of an .obj file
 *
 * Parameters:
 *
 *   command_info - Command information list
 *   pos          - Current position in command information
 *   p            - String containing line
 *   p_len        - Line length
 *   flags        - Reading flags
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_UNKNOWN_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_ERROR_MALFORMED_PARAMETER>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_parse_obj_line(CommandInformation *command_info, int pos,
                                  const char *p, size_t p_len, int flags) {
  char linebuf[4096];
  const char *token;
  int retval = TINYOBJ_ERROR_NOT_SET;
  Command *command = NULL;
  assert(p_len < 4095);

  memcpy(linebuf, p, p_len);
  linebuf[p_len] = '\0';

  token = linebuf;
  command = &command_info->command_list[pos];
  command->type = COMMAND_EMPTY;

  /* Skip leading space. */
  skip_space(&token);

  assert(token);
  if (token[0] == '\0' || token[0] == '\r')
    return TINYOBJ_NO_COMMAND;                     // Empty line
  if (token[0] == '#') return TINYOBJ_NO_COMMAND;  // Comment line

  /**
   * Initial parameter parsing
   **/
  switch (token[0]) {
    /**
     * Vertex data
     **/
    case 'v':  // v, vt, vn, vp
      retval = tinyobj_obj_parse_vertex(command, &token);
      break;
    /**
     * @todo {
     *  . rational or non-rational forms of curve or surface type:
     *     basis matrix, Bezier, B-spline, Cardinal, Taylor (cstype)
     *  . degree (deg)
     *  . basis matrix (bmat)
     *  . step size (step)
     * }
     **/
    /**
     * Elements
     **/
    case 'f':  // Faces
      retval = tinyobj_obj_parse_face(command, &token,
                                      flags & TINYOBJ_FLAG_TRIANGULATE);
      break;
    case 'l':  // Lines
      retval = tinyobj_obj_parse_line(command, &token);
    case 'p':  // Points
      retval = tinyobj_obj_parse_point(command, &token);
      break;
    // @todo { curve (curv);  2D curve (curv2); surface (surf) }
    /**
     * Free-form curve/surface body statements
     **/
    /**
     * @todo {
     *  . parameter values (parm)
     *  . outer trimming loop (trim)
     *  . inner trimming loop (hole)
     *  . special curve (scrv)
     *  . special point (sp)
     *  . end statement (end)
     * }
     **/
    /**
     * Connectivity between free-form surfaces
     **/
    // @todo { connect (con) }
    /**
     * Grouping
     **/
    case 'g':  // Group name
      /* @todo { multiple group name. } */
      if (!IS_SPACE(token[1])) {
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
      }
      token += 2;
      command->info.g.name = p + (token - linebuf);
      command->info.g.len =
          length_until_newline_comment_space(token, p_len - (token - linebuf)) +
          1;
      command->type = COMMAND_G;
      retval = TINYOBJ_SUCCESS;
      break;
    case 'o':  // Object name
      /* @todo { multiple object name? } */
      if (!IS_SPACE(token[1])) {
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
      }
      token += 2;
      command->info.o.name = p + (token - linebuf);
      command->info.o.len =
          length_until_newline_comment_space(token, p_len - (token - linebuf)) +
          1;
      command->type = COMMAND_O;
      retval = TINYOBJ_SUCCESS;
      break;
    case 's':  // Smoothing group
      if (!IS_SPACE(token[1])) {
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
      }
      token += 2;
      command->type = COMMAND_S;
      if (IS_DIGIT(token[0]))
        command->info.smoothing_id = parseInt(&token);
      else
        command->info.smoothing_id = (token[1] == 'f') ? 0 : 1;
      retval = TINYOBJ_SUCCESS;
      break;
    // @todo { merging group (mg) }
    /**
     * Display/Render attributes
     **/
    case 'u':  // Material name
      if (strncmp(token, "usemtl", 6) || !IS_SPACE((token[6]))) {
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
      }
      token += 7;
      skip_space(&token);
      command->info.usemtl.name = p + (token - linebuf);
      command->info.usemtl.len = length_until_newline_comment_space(
          token, (token, (p_len - (token - linebuf)) + 1));
      command->type = COMMAND_USEMTL;
      retval = TINYOBJ_SUCCESS;
      break;
    case 'm':  // Material library
      if (strncmp(token, "mtllib", 6) || !IS_SPACE((token[6]))) {
        retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
        break;
      }
      // By specification, `mtllib` should be appear only once in .obj
      token += 7;
      skip_space(&token);
      command->info.mtllib.name = p + (token - linebuf);
      command->info.mtllib.len =
          length_until_newline_comment_space(token, p_len - (token - linebuf)) +
          1;
      command->type = COMMAND_MTLLIB;
      retval = TINYOBJ_SUCCESS;
      break;
    /**
     * @todo {
     *  . bevel interpolation (bevel)
     *  . color interpolation (c_interp)
     *  . dissolve interpolation (d_interp)
     *  . level of detail (lod)
     *  . shadow casting (shadow_obj)
     *  . ray tracing (trace_obj)
     *  . curve approximation technique (ctech)
     *  . surface approximation technique (stech)
     * }
     **/
    default:
      retval = TINYOBJ_ERROR_UNKNOWN_PARAMETER;
      break;
  }
  if (retval == TINYOBJ_ERROR_UNKNOWN_PARAMETER) {
    char *command_name = NULL;
    size_t length;
    length = length_until_space(token, (p_len - (token - linebuf)) + 1);
    command_name = strndup(token, length);
    fprintf(stderr, "%s: Unsupported command '%s'\n", __func__, command_name);
    TINYOBJ_FREE(command_name);
    return retval;
  }
  // Increment specific counters
  switch (command->type) {
    case COMMAND_V:
      command_info->counter.v++;
      break;
    case COMMAND_VN:
      command_info->counter.vn++;
      break;
    case COMMAND_VT:
      command_info->counter.vt++;
      break;
    case COMMAND_VP:
      command_info->counter.vp++;
      break;
    case COMMAND_F:
      command_info->counter.f++;
      break;
    case COMMAND_P:
      command_info->counter.p += command->info.p.length;
      break;
    case COMMAND_L:
      command_info->counter.l++;
      break;

    case COMMAND_G:
    case COMMAND_O:
      command_info->counter.shapes++;
      break;
    case COMMAND_MTLLIB:
      command_info->mtllib_line_index = pos;
      break;
    default:
      break;
  }
  return retval;
}

/* Function: tinyobj_parse_obj_line_traverse
 *
 * Traverses and parses all lines of given buffer
 *
 * Parameters:
 *
 *  buf           - Buffer to be parsed
 *  len           - Buffer length
 *  command_info  - Command information to be filled
 *  flags         - Parsing flags
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_EMPTY>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_SUCCESS>
 */
static int tinyobj_parse_obj_line_traverse(const char *buf, size_t len,
                                           CommandInformation *command_info,
                                           unsigned int flags) {
  size_t i;
  unsigned int pos;
  size_t end_idx = len;
  unsigned int prev_pos = 0;
  unsigned int line_no = 0;
  size_t last_line_ending = 0;
  unsigned int num_lines = 0;

  // Count number of lines
  for (i = 0; i < end_idx; i++) {
    if (is_line_ending(buf, i, end_idx)) {
      num_lines++;
      last_line_ending = i;
    }
  }
  /**
   * The last char from the input may not be a line
   * ending character so add an extra line if there
   * are more characters after the last line ending
   * that was found.
   **/
  if (end_idx - last_line_ending > 0) num_lines++;

  if (num_lines == 0) return TINYOBJ_ERROR_EMPTY;

  command_info->command_list =
      TINYOBJ_MALLOC(sizeof(*command_info->command_list) * num_lines);
  if (!command_info->command_list) return TINYOBJ_ERROR_MEMORY;
  command_info->mtllib_line_index = -1;
  command_info->count = 0;
  memset(&command_info->counter, 0, sizeof(command_info->counter));

  pos = 0;
  for (i = 0; i < end_idx; i++) {
    if (is_line_ending(buf, i, end_idx)) {
      int ret;
      ret = tinyobj_parse_obj_line(command_info, pos, &buf[prev_pos],
                                   i - prev_pos, flags);

      prev_pos = i + 1;
      line_no++;
      if (ret == TINYOBJ_SUCCESS || ret == TINYOBJ_NO_COMMAND) {
        pos++;
        assert(pos < num_lines);
        command_info->count++;
      } else
        fprintf(stderr, "%s: Failed to parse line (%d): TINYOBJ ERROR: %d\n",
                __func__, line_no, ret);
    }
  }
  if (end_idx - last_line_ending > 0) {
    int ret;
    assert(pos < num_lines);
    ret = tinyobj_parse_obj_line(command_info, pos, &buf[prev_pos],
                                 end_idx - 1 - last_line_ending, flags);
    if (ret == TINYOBJ_SUCCESS) command_info->count++;
  }
  return TINYOBJ_SUCCESS;
}

/* Function: tinyobj_command_info_free
 * Frees given command information
 */
static void tinyobj_command_info_free(CommandInformation *command_info) {
  if (!command_info) return;
  if (command_info->command_list) TINYOBJ_FREE(command_info->command_list);
}

/* Function: tinyobj_shape_construct
 *
 * Constructs shape information
 *
 * Parameters:
 *
 *  shapes       - [in/out] Pointer to be filled
 *  num_shapes   - [out] Length of shapes
 *  command_info - Information used to construct the shapes
 */
static void tinyobj_shape_construct(tinyobj_shape_t **shapes,
                                    unsigned int *num_shapes,
                                    CommandInformation *command_info) {
  unsigned int face_count = 0;

  const char *shape_name = NULL;
  size_t shape_name_len = 0;
  unsigned int shape_idx = 0;

  const char *prev_shape_name = NULL;
  size_t prev_shape_name_len = 0;
  unsigned int prev_shape_face_offset = 0;
  unsigned int prev_face_offset = 0;
  tinyobj_shape_t prev_shape = {NULL, 0, 0};

  Command *command_list = command_info->command_list;
  unsigned int i;

  /**
   * Allocate array of shapes with maximum possible size(+1 for unnamed
   * group/object).
   * Actual # of shapes found in .obj is determined later
   **/
  (*shapes) =
      TINYOBJ_MALLOC(sizeof(**shapes) * (command_info->counter.shapes + 1));

  for (i = 0; i < command_info->count; i++) {
    switch (command_list[i].type) {
      case COMMAND_O:
      case COMMAND_G:
        if (command_list[i].type == COMMAND_O) {
          shape_name = command_list[i].info.o.name;
          shape_name_len = command_list[i].info.o.len;
        } else {
          shape_name = command_list[i].info.g.name;
          shape_name_len = command_list[i].info.g.len;
        }
        if (face_count == 0) {
          // 'o' or 'g' appears before any 'f'
          prev_shape_name = shape_name;
          prev_shape_name_len = shape_name_len;
          prev_shape_face_offset = face_count;
          prev_face_offset = face_count;
          continue;
        }
        if (shape_idx == 0) {
          // 'o' or 'g' after some 'v' lines
          (*shapes)[shape_idx].name =
              strndup(prev_shape_name, prev_shape_name_len);  // may be NULL
          (*shapes)[shape_idx].face_offset = prev_shape.face_offset;
          (*shapes)[shape_idx].length = face_count - prev_face_offset;
          shape_idx++;
          prev_face_offset = face_count;
        } else if ((face_count - prev_face_offset) > 0) {
          (*shapes)[shape_idx].name =
              strndup(prev_shape_name, prev_shape_name_len);
          (*shapes)[shape_idx].face_offset = prev_face_offset;
          (*shapes)[shape_idx].length = face_count - prev_face_offset;
          shape_idx++;
          prev_face_offset = face_count;
        }
        // Record shape info for succeeding 'o' or 'g' command
        prev_shape_name = shape_name;
        prev_shape_name_len = shape_name_len;
        prev_shape_face_offset = face_count;
        continue;
      case COMMAND_F:
        face_count++;
        continue;
      default:
        continue;
    }
  }

  if ((face_count - prev_face_offset) > 0) {
    unsigned int length = face_count - prev_shape_face_offset;
    if (length > 0) {
      (*shapes)[shape_idx].name = strndup(prev_shape_name, prev_shape_name_len);
      (*shapes)[shape_idx].face_offset = prev_face_offset;
      (*shapes)[shape_idx].length = face_count - prev_face_offset;
      shape_idx++;
    }
  } else {
    /**
     * Guess no 'v' line occurrence after 'o' or 'g', so discards current
     * shape information.
     **/
  }
  (*num_shapes) = shape_idx;
  TINYOBJ_FREE(*shapes);
  *shapes = NULL;
}

/* Function: tinyobj_shape_free
 *
 * Frees shape data
 *
 * Parameters:
 *
 *  shapes     - Shape information to be freed
 *  num_shapes - Length of shapes
 */
void tinyobj_shape_free(tinyobj_shape_t *shapes, unsigned int num_shapes) {
  unsigned int i;
  if (!shapes || !num_shapes) return;

  for (i = 0; i < num_shapes; i++) {
    if (shapes[i].name) TINYOBJ_FREE(shapes[i].name);
  }
  TINYOBJ_FREE(shapes);
}

/* Function: tinyobj_attrib_construct
 *
 * Constructs attribute data with given information
 *
 * Parameters:
 *
 *   attrib         - [in/out] Attribute to be filled
 *   command_info   - Information used to fill attrib
 *   material_table - Material table used (can be NULL)
 */
static void tinyobj_attrib_construct(tinyobj_attrib_t *attrib,
                                     CommandInformation *command_info,
                                     tinyobj_material_table_t *material_table) {
  int mtl_material_id =
      -1;  //< COMMAND_USEMTL material id defaults to -1 'Unknown material'
  int smoothing_id = 0;  //< Defaults to 0, no group
  struct {
    unsigned int v, vn, vt, vp;
    unsigned int f, /* p,*/ l;
  } count;
  unsigned int i;
  Command *command_list = command_info->command_list;
  memset(&count, 0, sizeof(count));

  attrib->v_count = command_info->counter.v;
  attrib->v = (attrib->v_count)
                  ? TINYOBJ_MALLOC(sizeof(*attrib->v) * attrib->v_count)
                  : NULL;

  attrib->vn_count = command_info->counter.vn;
  attrib->vn = (attrib->vn_count)
                   ? TINYOBJ_MALLOC(sizeof(*attrib->vn) * attrib->vn_count)
                   : NULL;

  attrib->vt_count = command_info->counter.vt;
  attrib->vt = (attrib->vt_count)
                   ? TINYOBJ_MALLOC(sizeof(*attrib->vt) * attrib->vt_count)
                   : NULL;

  attrib->vp_count = command_info->counter.vp;
  attrib->vp = (attrib->vp_count)
                   ? TINYOBJ_MALLOC(sizeof(*attrib->vp) * attrib->vp_count)
                   : NULL;

  attrib->f_count = command_info->counter.f;
  attrib->f = (attrib->f_count)
                  ? TINYOBJ_MALLOC(sizeof(*attrib->f) * attrib->f_count)
                  : NULL;
  attrib->triangle_count_total = 0;

  attrib->l_count = command_info->counter.l;
  attrib->l = (attrib->l_count)
                  ? TINYOBJ_MALLOC(sizeof(*attrib->l) * attrib->l_count)
                  : NULL;

  attrib->p.count = 0;
  attrib->p.length = command_info->counter.p;
  attrib->p.v_idx =
      (attrib->p.length)
          ? TINYOBJ_MALLOC(sizeof(*attrib->p.v_idx) * attrib->p.length)
          : NULL;

  for (i = 0; i < command_info->count; i++) {
    switch (command_list[i].type) {
      case COMMAND_USEMTL: {
        char *material_name_null_term;
        if (!command_list[i].info.usemtl.name ||
            !command_list[i].info.usemtl.len)
          continue;
        if (!material_table) continue;
        material_name_null_term =
            TINYOBJ_MALLOC(command_list[i].info.usemtl.len + 1);
        memcpy(material_name_null_term, command_list[i].info.usemtl.name,
               command_list[i].info.usemtl.len);
        material_name_null_term[command_list[i].info.usemtl.len] = '\0';
#ifndef TINYOBJ_USE_UTHASH
        if (hash_table_exists(material_name_null_term, material_table))
          mtl_material_id =
              hash_table_get(material_name_null_term, material_table);
        else
          mtl_material_id = -1;
#else
        {
          tinyobj_material_table_tEntry *entry = NULL;
          tinyobj_hash_find(material_table, material_name_null_term, &entry);
          mtl_material_id = (entry) ? entry->value : -1;
        }
#endif
        if (mtl_material_id < 0)
          fprintf(stderr, "%s: USEMTL %s not found in table!\n", __func__,
                  material_name_null_term);
        TINYOBJ_FREE(material_name_null_term);
        continue;
      }
      case COMMAND_S:
        smoothing_id = command_list[i].info.smoothing_id;
        continue;
      case COMMAND_V:
        assert(attrib->v != NULL);
        memcpy(&attrib->v[count.v], &command_list[i].info.v,
               sizeof(*attrib->v));
        count.v++;
        continue;
      case COMMAND_VN:
        assert(attrib->vn != NULL);
        memcpy(&attrib->vn[count.vn], &command_list[i].info.vn,
               sizeof(*attrib->vn));
        count.vn++;
        continue;
      case COMMAND_VT:
        assert(attrib->vt != NULL);
        memcpy(&attrib->vt[count.vt], &command_list[i].info.vt,
               sizeof(*attrib->vt));
        count.vt++;
        continue;
      case COMMAND_VP:
        assert(attrib->vp != NULL);
        memcpy(&attrib->vp[count.vp], &command_list[i].info.vp,
               sizeof(*attrib->vp));
        count.vp++;
        continue;
      case COMMAND_F: {
        unsigned int k = 0;
        assert(attrib->f != NULL);
        attrib->f[count.f].triangle_count =
            command_list[i].info.f.triangle_count;
        attrib->f[count.f].triplet_list = command_list[i].info.f.triplet_list;
        attrib->f[count.f].count = command_list[i].info.f.count;
        attrib->f[count.f].length = command_list[i].info.f.length;
        // Fix triplet index
        for (k = 0; k < command_list[i].info.f.count; k++) {
          tinyobj_vertex_index_t *vi = &command_list[i].info.f.triplet_list[k];
          attrib->f[count.f].triplet_list[k].v_idx =
              fixIndex(vi->v_idx, count.v);
          attrib->f[count.f].triplet_list[k].vt_idx =
              fixIndex(vi->vt_idx, count.vt);
          attrib->f[count.f].triplet_list[k].vn_idx =
              fixIndex(vi->vn_idx, count.vn);
        }
        attrib->f[count.f].material_id = mtl_material_id;
        attrib->f[count.f].smoothing_id = smoothing_id;
        count.f++;
        attrib->triangle_count_total += command_list[i].info.f.triangle_count;
        continue;
      }
      case COMMAND_P: {
        size_t copy_len = command_list[i].info.p.count;
        assert(attrib->p.v_idx != NULL);
        if (copy_len + attrib->p.count > attrib->p.length) {
          fprintf(stderr, "%s: Point count is greater than counter length!\n",
                  __func__);
          continue;
        }
        memcpy(&attrib->p.v_idx[attrib->p.count], command_list[i].info.p.v_idx,
               sizeof(*attrib->p.v_idx) * copy_len);
        attrib->p.count += copy_len;
        tinyobj_obj_free_point(&command_list[i].info.p);
        continue;
      }
      case COMMAND_L: {
        unsigned int k = 0;
        assert(attrib->l != NULL);
        attrib->l[count.l].couple_list = command_list[i].info.l.couple_list;
        attrib->l[count.l].count = command_list[i].info.l.count;
        attrib->l[count.l].length = command_list[i].info.l.length;
        // Fix couple index
        for (k = 0; k < command_list[i].info.l.count; k++) {
          attrib->l[count.l].couple_list[k].v_idx =
              fixIndex(command_list[i].info.l.couple_list[k].v_idx, count.v);
          attrib->l[count.l].couple_list[k].vt_idx =
              fixIndex(command_list[i].info.l.couple_list[k].vt_idx, count.vt);
        }
        count.f++;
      }
      default:
      case COMMAND_EMPTY:
        continue;
    }
  }
}

// Group: Aplication Programming Interface (.obj)

/* Function: tinyobj_attrib_free
 * Frees attribute data
 */
void tinyobj_attrib_free(tinyobj_attrib_t *attrib) {
  unsigned int i;
  if (!attrib) return;
  if (attrib->v) TINYOBJ_FREE(attrib->v);
  if (attrib->vn) TINYOBJ_FREE(attrib->vn);
  if (attrib->vt) TINYOBJ_FREE(attrib->vt);
  if (attrib->vp) TINYOBJ_FREE(attrib->vp);
  if (attrib->f) {
    for (i = 0; i < attrib->f_count; i++) tinyobj_obj_free_face(&attrib->f[i]);
  }
  if (attrib->l) {
    for (i = 0; i < attrib->l_count; i++) tinyobj_obj_free_line(&attrib->l[i]);
  }
  if (attrib->p.v_idx) tinyobj_obj_free_point(&attrib->p);
}

/* Function: tinyobj_attrib_init
 * Initializes given attribute information to default values
 */
static void tinyobj_attrib_init(tinyobj_attrib_t *attrib) {
  if (!attrib) return;
  attrib->f = NULL;
  attrib->l = NULL;
  attrib->v = NULL;
  attrib->vn = NULL;
  attrib->vp = NULL;
  attrib->vt = NULL;
  attrib->p.v_idx = NULL;
  attrib->p.count = 0;
  attrib->p.length = 0;

  attrib->f_count  = 0;
  attrib->l_count  = 0;
  attrib->v_count  = 0;
  attrib->vn_count = 0;
  attrib->vp_count = 0;
  attrib->vt_count = 0;
  // memset(&attrib, 0, sizeof(*attrib));
}

/* Function: tinyobj_parse_obj
 *
 *  Parses .obj file
 *
 * Parameters:
 *
 *  attrib             - [in/out] Attributes to be filled
 *  shapes             - [in/out] Shapes to be filled
 *  num_shapes         - [in/out] Length of shapes
 *  materials_out      - [in/out] Materials to be filled
 *  num_materials_out  - [in/out] Length of materials
 *  buf                - Buffer to be read
 *  len                - Buffer length
 *  flags              - Parsing flags
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_INVALID_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_SUCCESS>
 */
int tinyobj_parse_obj(tinyobj_attrib_t *attrib, tinyobj_shape_t **shapes,
                      unsigned int *num_shapes,
                      tinyobj_material_t **materials_out,
                      unsigned int *num_materials_out, const char *buf,
                      size_t len, unsigned int flags) {
  CommandInformation command_info;
  Command *command_list = NULL;
  tinyobj_material_table_t material_table;

  tinyobj_material_t *materials = NULL;
  unsigned int num_materials = 0;
  int ret;

  // Verify if parameters are valid
  if (len < 1 || buf == NULL || attrib == NULL || shapes == NULL ||
      num_shapes == NULL || materials_out == NULL ||
      num_materials_out == NULL) {
    fprintf(stderr, "%s: Failed to parse object, invalid parameters\n",
            __func__);
    return TINYOBJ_ERROR_INVALID_PARAMETER;
  }

  tinyobj_attrib_init(attrib);
  // Traverse lines and fill command information
  command_info.command_list = NULL;
  ret = tinyobj_parse_obj_line_traverse(buf, len, &command_info, flags);
  if (ret != TINYOBJ_SUCCESS) {
    tinyobj_command_info_free(&command_info);
    fprintf(stderr, "%s: Failed to traverse lines for given object: %d\n",
            __func__, ret);
    return ret;
  }
  command_list = command_info.command_list;
#ifndef TINYOBJ_USE_UTHASH
  create_hash_table(TINYOBJ_HASH_TABLE_DEFAULT_SIZE, &material_table);
#else
  tinyobj_hash_init(&material_table);
#endif

  // Load material if 'mtllib' is defined
  if (command_info.mtllib_line_index >= 0 &&
      command_list[command_info.mtllib_line_index].type == COMMAND_MTLLIB &&
      command_list[command_info.mtllib_line_index].info.mtllib.len > 0) {
    char *filename =
        strndup(command_list[command_info.mtllib_line_index].info.mtllib.name,
                command_list[command_info.mtllib_line_index].info.mtllib.len);
    ret = tinyobj_parse_and_index_mtl_file(&materials, &num_materials, filename,
                                           &material_table);
    if (ret != TINYOBJ_SUCCESS) {
      fprintf(stderr, "%s: Failed to parse material file '%s': '%d'\n",
              __func__, filename, ret);
    }
    TINYOBJ_FREE(filename);
  }

  // Construct attributes
  tinyobj_attrib_construct(attrib, &command_info, &material_table);
#ifndef TINYOBJ_USE_UTHASH
  destroy_hash_table(&material_table);
#else
  tinyobj_hash_free(&material_table, 0);
#endif

  tinyobj_shape_construct(shapes, num_shapes, &command_info);

  tinyobj_command_info_free(&command_info);
  (*materials_out) = materials;
  (*num_materials_out) = num_materials;

  return TINYOBJ_SUCCESS;
}

#ifdef TINYOBJ_ENABLE_OLDER_ATTRIBUTE
// Group: Compatibility with older versions (deprecated)

/* Function: tinyobj_attrib_free_compat
 * Frees given attribute
 */
void tinyobj_attrib_free_compat(COMPATtinyobj_attrib_t *attrib) {
  if (!attrib) return;
  if (attrib->vertices) TINYOBJ_FREE(attrib->vertices);
  if (attrib->normals) TINYOBJ_FREE(attrib->normals);
  if (attrib->texcoords) TINYOBJ_FREE(attrib->texcoords);
  if (attrib->faces) TINYOBJ_FREE(attrib->faces);
  if (attrib->face_num_verts) TINYOBJ_FREE(attrib->face_num_verts);
  if (attrib->material_ids) TINYOBJ_FREE(attrib->material_ids);
}

/* Function: tinyobj_new2old
 *
 * Converts new attribute format to the old one
 * This expects that the faces were triangulated
 *
 * Parameters:
 *
 *    attrib     - [in] Attribute to be converted
 *    out_attrib - [out] Attribute to be filled
 *
 * Returns:
 *
 *  - <TINYOBJ_ERROR_INVALID_PARAMETER>
 *  - <TINYOBJ_ERROR_MEMORY>
 *  - <TINYOBJ_SUCCESS>
 */
int tinyobj_new2old(tinyobj_attrib_t *attrib,
                    COMPATtinyobj_attrib_t *out_attrib) {
  unsigned int i, j, face_pos, l;

  if (!attrib || !out_attrib) return TINYOBJ_ERROR_INVALID_PARAMETER;

  // Initialize old attributes
  out_attrib->vertices = NULL;
  out_attrib->num_vertices = 0;
  out_attrib->normals = NULL;
  out_attrib->num_normals = 0;
  out_attrib->texcoords = NULL;
  out_attrib->num_texcoords = 0;
  out_attrib->faces = NULL;
  out_attrib->num_faces = 0;
  out_attrib->face_num_verts = NULL;
  out_attrib->num_face_num_verts = 0;
  out_attrib->material_ids = NULL;

  assert(attrib->v_count > 0);
  out_attrib->num_vertices = attrib->v_count;
  out_attrib->vertices = TINYOBJ_MALLOC(sizeof(*out_attrib->vertices) *
                                        out_attrib->num_vertices * 3);
  if (!out_attrib->vertices) return TINYOBJ_ERROR_MEMORY;
  for (i = 0, j = 0; i < attrib->v_count; i++, j += 3) {
    out_attrib->vertices[j + 0] = attrib->v[i].x;
    out_attrib->vertices[j + 1] = attrib->v[i].y;
    out_attrib->vertices[j + 2] = attrib->v[i].z;
  }

  if (attrib->vn_count) {
    out_attrib->num_normals = attrib->vn_count;
    out_attrib->normals = TINYOBJ_MALLOC(sizeof(*out_attrib->normals) *
                                         out_attrib->num_normals * 3);
    if (!out_attrib->normals) {
      tinyobj_attrib_free_compat(out_attrib);
      return TINYOBJ_ERROR_MEMORY;
    }
    for (i = 0, j = 0; i < attrib->vn_count; i++, j += 3) {
      out_attrib->normals[j + 0] = attrib->vn[i].i;
      out_attrib->normals[j + 1] = attrib->vn[i].j;
      out_attrib->normals[j + 2] = attrib->vn[i].k;
    }
  }
  if (attrib->vt_count) {
    out_attrib->num_texcoords = attrib->vt_count;
    out_attrib->texcoords = TINYOBJ_MALLOC(sizeof(*out_attrib->texcoords) *
                                           out_attrib->num_texcoords * 2);
    if (!out_attrib->texcoords) {
      tinyobj_attrib_free_compat(out_attrib);
      return TINYOBJ_ERROR_MEMORY;
    }
    for (i = 0, j = 0; i < attrib->vt_count; i++, j += 2) {
      out_attrib->texcoords[j + 0] = attrib->vt[i].u;
      out_attrib->texcoords[j + 1] = attrib->vt[i].v;
    }
  }

  out_attrib->num_faces = 0;
  for (i = 0; i < attrib->f_count; i++)
    out_attrib->num_faces += attrib->f[i].count;
  out_attrib->faces =
      TINYOBJ_MALLOC(sizeof(*out_attrib->faces) * out_attrib->num_faces);
  out_attrib->material_ids =
      TINYOBJ_MALLOC(sizeof(*out_attrib->material_ids) * attrib->f_count);
  out_attrib->num_face_num_verts = attrib->triangle_count_total;
  out_attrib->face_num_verts = TINYOBJ_MALLOC(
      sizeof(*out_attrib->face_num_verts) * out_attrib->num_face_num_verts);
  if (!out_attrib->faces || !out_attrib->face_num_verts ||
      !out_attrib->material_ids) {
    tinyobj_attrib_free_compat(out_attrib);
    return TINYOBJ_ERROR_MEMORY;
  }
  face_pos = 0;
  l = 0;
  for (i = 0; i < attrib->f_count; i++) {
    unsigned int m;
    for (j = 0; j < attrib->f[i].count; j++) {
      out_attrib->faces[face_pos].v_idx = attrib->f[i].triplet_list[j].v_idx;
      out_attrib->faces[face_pos].vn_idx = attrib->f[i].triplet_list[j].vn_idx;
      out_attrib->faces[face_pos].vt_idx = attrib->f[i].triplet_list[j].vt_idx;
      face_pos++;
    }
    if (attrib->f[i].count % 3 == 0) {
      for (m = 0; m < attrib->f[i].triangle_count; m++) {
        out_attrib->face_num_verts[l] = 3;
        l++;
      }
    } else {
      out_attrib->face_num_verts[l] = 1;
      l++;
    }
    out_attrib->material_ids[i] = attrib->f[i].material_id;
  }

  return TINYOBJ_SUCCESS;
}
#endif  // TINYOBJ_ENABLE_OLDER_ATTRIBUTE

#endif  // TINYOBJ_LOADER_C_IMPLEMENTATION

#endif  // TINOBJ_LOADER_C_H_
