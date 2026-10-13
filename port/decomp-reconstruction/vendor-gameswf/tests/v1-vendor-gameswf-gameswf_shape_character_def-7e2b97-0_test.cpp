// Test for batch v1-vendor-gameswf-gameswf_shape_character_def-7e2b97-0.
//
// Covers the arithmetic and selection rules of shape_character_def::display (0x77a990) through the
// pure helpers in vendor-gameswf/gameswf_shape_dh2_helpers.hpp. The gameswf classes are not needed.
// Returns non-zero when any check fails.

#include <cmath>
#include <cstdio>

#include "vendor-gameswf/gameswf_shape_dh2_helpers.hpp"

namespace {

int g_failures = 0;

void check(bool ok, const char* what)
{
	if (!ok)
	{
		++g_failures;
		std::printf("FAIL: %s\n", what);
	}
}

void check_near(float got, float want, const char* what)
{
	check(std::fabs(got - want) <= 1e-5f * (std::fabs(want) + 1.0f), what);
}

void test_max_scale()
{
	using gameswf::dh2::shape_matrix_max_scale;
	check_near(shape_matrix_max_scale(3.0f, 4.0f, 0.0f, 0.0f), 5.0f, "max scale, first basis dominates");
	check_near(shape_matrix_max_scale(1.0f, 0.0f, 0.0f, 2.0f), 2.0f, "max scale, second basis dominates");
	check_near(shape_matrix_max_scale(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, "max scale, zero matrix");
	check_near(shape_matrix_max_scale(2.0f, 0.0f, 0.0f, 2.0f), 2.0f, "max scale, tie");
	check_near(shape_matrix_max_scale(0.0f, 1.0f, 1.0f, 0.0f), 1.0f, "max scale, rotation has unit scale");
	// tu_fmax(s1, s2) = (s1 < s2) ? s2 : s1: a NaN first basis is kept, a NaN second basis is ignored.
	check(std::isnan(shape_matrix_max_scale(std::nanf(""), 0.0f, 0.0f, 2.0f)), "max scale, NaN first basis propagates");
	check_near(shape_matrix_max_scale(1.0f, 0.0f, 0.0f, std::nanf("")), 1.0f, "max scale, NaN second basis is ignored");
}

void test_drawable()
{
	using gameswf::dh2::shape_scale_is_drawable;
	check(!shape_scale_is_drawable(0.0f), "scale 0 is not drawable");
	check(!shape_scale_is_drawable(1e-7f), "scale below 1e-6 is not drawable");
	check(!shape_scale_is_drawable(std::nextafter(1e-6f, 0.0f)), "scale just below 1e-6f is not drawable");
	check(shape_scale_is_drawable(1e-6f), "scale 1e-6f is drawable (the binary returns only when less than 1e-6f)");
	check(shape_scale_is_drawable(0.5f), "scale 0.5 is drawable");
	check(shape_scale_is_drawable(-0.5f), "negative scale uses its magnitude");
	check(shape_scale_is_drawable(std::nanf("")), "NaN scale is drawable (fcmplt is false for NaN, so the binary keeps drawing)");
}

void test_error_and_tolerance()
{
	using namespace gameswf::dh2;
	check_near(shape_object_space_error(2.0f, 1.0f, 1.0f), 10.0f, "error at scale 2, pixel 1");
	check_near(shape_object_space_error(2.0f, 2.0f, 1.0f), 5.0f, "error halves with pixel scale");
	check_near(shape_object_space_error(2.0f, 1.0f, 3.0f), 30.0f, "error follows curve max pixel error");
	check_near(shape_new_mesh_tolerance(10.0f), 7.5f, "new mesh tolerance is 0.75 of the error");
}

// Cached meshes with tolerances 4, 2, 1 (highest error first), as the sort leaves them.
void test_select_from_array()
{
	using gameswf::dh2::shape_select_cached_mesh;
	const float tol[3] = {4.0f, 2.0f, 1.0f};
	auto at = [&tol](int i) { return tol[i]; };

	check(shape_select_cached_mesh(3, 100.0f, at) == -1, "error far above the first mesh builds a new mesh");
	check(shape_select_cached_mesh(3, 5.0f, at) == 0, "error above the first tolerance uses mesh 0");
	check(shape_select_cached_mesh(3, 3.0f, at) == 1, "error between tolerances uses mesh 1");
	check(shape_select_cached_mesh(3, 1.5f, at) == 2, "error above the finest tolerance uses mesh 2");
	check(shape_select_cached_mesh(3, 0.5f, at) == -1, "error below every tolerance builds a new mesh");
	check(shape_select_cached_mesh(0, 5.0f, at) == -1, "no cached meshes builds a new mesh");
	check(shape_select_cached_mesh(3, std::nanf(""), at) == -1, "NaN error builds a new mesh");

	// A NaN first tolerance makes both first-mesh tests false, so the binary moves on to mesh 1.
	const float nan_first[2] = {std::nanf(""), 2.0f};
	auto at_nan = [&nan_first](int i) { return nan_first[i]; };
	check(shape_select_cached_mesh(2, 5.0f, at_nan) == 1, "a NaN first tolerance is passed over, and mesh 1 is shown");
}

void test_select_stops_on_coarse_mesh()
{
	using gameswf::dh2::shape_select_cached_mesh;
	const float coarse_then_fine[2] = {10.0f, 1.0f};
	auto at = [&coarse_then_fine](int i) { return coarse_then_fine[i]; };
	// Mesh 0 (tol 10) is too coarse to show, but mesh 1 (tol 1) has 1 * 3 < 5: more than 3x finer than
	// needed, so the search stops there and a new mesh is built.
	check(shape_select_cached_mesh(2, 5.0f, at) == -1, "a mesh more than 3x finer than needed stops the search");

	const float single[1] = {1.0f};
	auto one = [&single](int i) { return single[i]; };
	check(shape_select_cached_mesh(1, 10.0f, one) == -1, "a first mesh more than 3x finer than needed is not reused");
}

}  // namespace

int main()
{
	test_max_scale();
	test_drawable();
	test_error_and_tolerance();
	test_select_from_array();
	test_select_stops_on_coarse_mesh();
	if (g_failures != 0)
	{
		std::printf("%d check(s) failed\n", g_failures);
		return 1;
	}
	std::printf("shape_character_def helpers: all checks passed\n");
	return 0;
}
