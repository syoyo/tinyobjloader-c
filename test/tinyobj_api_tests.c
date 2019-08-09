#define TEST_NO_MAIN

#include "tinyobj_api_tests.h"
#include "tinyobj_loader_c.h"
#include "acutest.h"

size_t loadFile(const char *filename, char **buffer) {
  long string_size = 0, read_size = 0;
  FILE *handler = fopen(filename, "r");
  *buffer = NULL;

  if (handler) {
    if (fseek(handler, 0, SEEK_END) != 0) {
      fprintf(stderr, "loadFile: fseek %s failure (errno %d)\n", filename,
              errno);
      return 0;
    }
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char *)malloc(sizeof(char) * (string_size + 1));
    read_size = (long)fread(*buffer, sizeof(char), string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size && errno) {
      fprintf(stderr, "loadFile: fread %s failure (errno %d)\n", filename,
              errno);
      free(*buffer);
      *buffer = NULL;
    }
    fclose(handler);
  }
  return (size_t)read_size;
}

void test_tinyobj_parse_mtl_file(void)
{
    {
        const char * filename = "fixtures/cube.mtl";

        tinyobj_material_t * material;
        unsigned int num_materials;
        int ret;

		ret = tinyobj_parse_mtl_file(&material, &num_materials, filename);
        TEST_CHECK(ret == TINYOBJ_SUCCESS);
        if(ret != TINYOBJ_SUCCESS)
			return;

        TEST_CHECK(num_materials == 1);
        TEST_MSG("num_materials %ld\n", num_materials);
        TEST_CHECK(strcmp(material[0].name, "CubeMaterial") == 0);
        TEST_MSG("material name: %s\n", material[0].name);

        TEST_CHECK(material[0].diffuse.r == 1.0);
        TEST_CHECK(material[0].diffuse.g == 0.0);
        TEST_CHECK(material[0].diffuse.b == 0.0);

        TEST_CHECK(material[0].specular.r == 0.5);
        TEST_CHECK(material[0].specular.g == 0.25);
        TEST_CHECK(material[0].specular.b == 0.125);

        TEST_CHECK(material[0].ambient.r == 1.0);
        TEST_CHECK(material[0].ambient.g == 1.0);
        TEST_CHECK(material[0].ambient.b == 1.0);

        TEST_CHECK(material[0].emission.r == 0.0);
        TEST_CHECK(material[0].emission.g == 1.0);
        TEST_CHECK(material[0].emission.b == 0.0);

        TEST_CHECK(material[0].illum == 2);
        TEST_CHECK(material[0].dissolve == 1.0);
    }
}

tinyobj_vertex_normal_t vertex_normal_test_vector[] = {
	{ 0.0f, -1.0f,  0.0f}, // vn1
	{ 0.0f,  1.0f,  0.0f}, // vn2
	{ 1.0f,  0.0f,  0.0f}, // vn3
	{-0.0f, -0.0f,  1.0f}, // vn4
	{-1.0f, -0.0f, -0.0f}, // vn5
	{ 0.0f,  0.0f, -1.0f}, // vn6
};

tinyobj_vertex_t vertex_test_vector[] = {
	{ 1.0f,      -1.0f, -1.0f,      1.0f}, // v1
	{ 1.0f,      -1.0f,  1.0f,      1.0f}, // v2
	{-1.0f,      -1.0f,  1.0f,      1.0f}, // v3
	{-1.0f,      -1.0f, -1.0f,      1.0f}, // v4
	{ 1.0f,       1.0f, -0.999999f, 1.0f}, // v5
	{ 0.999999f,  1.0f,  1.000001f, 1.0f}, // v6
	{-1.0f,       1.0f,  1.0f,      1.0f}, // v7
	{-1.0f,       1.0f, -1.0f,      1.0f}, // v8
};

/**
 * Test vector containing the triplets of all faces (without triangulation)
 **/
tinyobj_vertex_index_t vi_test_vector[] = {
	// Face 1
	{1, TINYOBJ_INVALID_INDEX, 1}, {2, TINYOBJ_INVALID_INDEX, 1},
	{3, TINYOBJ_INVALID_INDEX, 1}, {4, TINYOBJ_INVALID_INDEX, 1},
	// Face 2
	{5, TINYOBJ_INVALID_INDEX, 2}, {8, TINYOBJ_INVALID_INDEX, 2},
	{7, TINYOBJ_INVALID_INDEX, 2}, {6, TINYOBJ_INVALID_INDEX, 2},
	// Face 3
	{1, TINYOBJ_INVALID_INDEX, 3}, {5, TINYOBJ_INVALID_INDEX, 3},
	{6, TINYOBJ_INVALID_INDEX, 3}, {2, TINYOBJ_INVALID_INDEX, 3},
	// Face 4
	{2, TINYOBJ_INVALID_INDEX, 4}, {6, TINYOBJ_INVALID_INDEX, 4},
	{7, TINYOBJ_INVALID_INDEX, 4}, {3, TINYOBJ_INVALID_INDEX, 4},
	// Face 5
	{3, TINYOBJ_INVALID_INDEX, 5}, {7, TINYOBJ_INVALID_INDEX, 5},
	{8, TINYOBJ_INVALID_INDEX, 5}, {4, TINYOBJ_INVALID_INDEX, 5},
	// Face 6
	{5, TINYOBJ_INVALID_INDEX, 6}, {1, TINYOBJ_INVALID_INDEX, 6},
	{4, TINYOBJ_INVALID_INDEX, 6}, {8, TINYOBJ_INVALID_INDEX, 6},
};

/**
 * Test vector containing the triplets of all faces (triangulated)
 * (already fixed)
 **/
tinyobj_vertex_index_t vi_test_vector_trig[] = {
	// Face 1
	{0, TINYOBJ_INVALID_INDEX, 0}, {1, TINYOBJ_INVALID_INDEX, 0}, {2, TINYOBJ_INVALID_INDEX, 0},
	{0, TINYOBJ_INVALID_INDEX, 0}, {2, TINYOBJ_INVALID_INDEX, 0}, {3, TINYOBJ_INVALID_INDEX, 0},
	// Face 2
	{4, TINYOBJ_INVALID_INDEX, 1}, {7, TINYOBJ_INVALID_INDEX, 1}, {6, TINYOBJ_INVALID_INDEX, 1},
	{4, TINYOBJ_INVALID_INDEX, 1}, {6, TINYOBJ_INVALID_INDEX, 1}, {5, TINYOBJ_INVALID_INDEX, 1},
	// Face 3
	{0, TINYOBJ_INVALID_INDEX, 2}, {4, TINYOBJ_INVALID_INDEX, 2}, {5, TINYOBJ_INVALID_INDEX, 2},
	{0, TINYOBJ_INVALID_INDEX, 2}, {5, TINYOBJ_INVALID_INDEX, 2}, {1, TINYOBJ_INVALID_INDEX, 2},
	// Face 4
	{1, TINYOBJ_INVALID_INDEX, 3}, {5, TINYOBJ_INVALID_INDEX, 3}, {6, TINYOBJ_INVALID_INDEX, 3},
	{1, TINYOBJ_INVALID_INDEX, 3}, {6, TINYOBJ_INVALID_INDEX, 3}, {2, TINYOBJ_INVALID_INDEX, 3},
	// Face 5
	{2, TINYOBJ_INVALID_INDEX, 4}, {6, TINYOBJ_INVALID_INDEX, 4}, {7, TINYOBJ_INVALID_INDEX, 4},
	{2, TINYOBJ_INVALID_INDEX, 4}, {7, TINYOBJ_INVALID_INDEX, 4}, {3, TINYOBJ_INVALID_INDEX, 4},
	// Face 6
	{4, TINYOBJ_INVALID_INDEX, 5}, {0, TINYOBJ_INVALID_INDEX, 5}, {3, TINYOBJ_INVALID_INDEX, 5},
	{4, TINYOBJ_INVALID_INDEX, 5}, {3, TINYOBJ_INVALID_INDEX, 5}, {7, TINYOBJ_INVALID_INDEX, 5},
};

unsigned char test_compare_vertex_texture(tinyobj_vertex_texture_t *vt, tinyobj_vertex_texture_t *vt1) {
	return (vt->u == vt1->u && vt->v == vt1->v && vt->w == vt1->w);
}

unsigned char test_compare_vertex_normal(tinyobj_vertex_normal_t *vn, tinyobj_vertex_normal_t *vn1) {
	return (vn->i == vn1->i && vn->j == vn1->j && vn->k == vn1->k);
}

unsigned char test_compare_vertex(tinyobj_vertex_t *v, tinyobj_vertex_t *v1) {
	return (v->x == v1->x && v->y == v1->y && v->z == v1->z && v->weight == v1->weight);
}

/**
 * Tests .obj parsing function
 **/
void test_tinyobj_parse_obj(void) {
	const char * filename = "fixtures/cube.obj";
	tinyobj_shape_t * shape = NULL;
	tinyobj_material_t * material = NULL;
	tinyobj_attrib_t attrib;

	unsigned int num_shapes;
	unsigned int num_materials;

	char * obj_contents;
	size_t file_size = loadFile(filename, &obj_contents);

	int result = tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials, obj_contents, file_size, TINYOBJ_FLAG_TRIANGULATE);
	TEST_CHECK(result == TINYOBJ_SUCCESS);
	if(result != TINYOBJ_SUCCESS)
		return;

	// Face tests
	TEST_CHECK(attrib.f_count == (sizeof(vi_test_vector)/sizeof(vi_test_vector[0]))/4);
	{ // Test if vertex indices match
		size_t i = 0;
		for(i = 0; i < attrib.f_count; i++) {
			size_t j = 0;
			TEST_CHECK(attrib.f[i].count == 6);
			TEST_CHECK(attrib.f[i].smoothing_id == 0);

			for(j = 0; j < attrib.f[i].count; j++) {
				size_t tpos = attrib.f[i].count*i+j;
				tinyobj_vertex_index_t vi;
				memcpy(&vi, &attrib.f[i].triplet_list[j], sizeof(vi));
				TEST_CASE("Triplet indices match test");
				TEST_CHECK(vi.v_idx  == vi_test_vector_trig[tpos].v_idx);
				TEST_CHECK(vi.vt_idx == vi_test_vector_trig[tpos].vt_idx
					|| vi.vt_idx == TINYOBJ_INVALID_INDEX && vi_test_vector_trig[tpos].vt_idx == TINYOBJ_INVALID_INDEX);
				TEST_CHECK(vi.vn_idx == vi_test_vector_trig[tpos].vn_idx
					|| vi.vn_idx == TINYOBJ_INVALID_INDEX && vi_test_vector_trig[tpos].vn_idx == TINYOBJ_INVALID_INDEX);
				TEST_CASE("Triplet values match test");
				TEST_CHECK(vi.v_idx < attrib.v_count);
				TEST_CHECK(test_compare_vertex(&attrib.v[vi.v_idx], &vertex_test_vector[vi.v_idx]));
				TEST_CHECK(vi.vn_idx < attrib.vn_count);
				TEST_CHECK(test_compare_vertex_normal(&attrib.vn[vi.vn_idx], &vertex_normal_test_vector[vi.vn_idx]));
			}
		}
	}
	TEST_CASE(NULL);

	TEST_CHECK(attrib.v_count  == 8);
	TEST_CHECK(attrib.vn_count == 6);
	TEST_CHECK(attrib.vt_count == 0);

	tinyobj_attrib_free(&attrib);
	tinyobj_shape_free(shape, num_shapes);
	tinyobj_material_free(material, num_materials);
}
