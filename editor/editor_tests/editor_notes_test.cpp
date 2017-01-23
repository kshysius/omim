#include "testing/testing.hpp"

#include "editor/editor_notes.hpp"

#include "geometry/mercator.hpp"

#include "platform/platform_tests_support/scoped_file.hpp"

#include "coding/file_name_utils.hpp"

#include "base/math.hpp"

using namespace editor;

UNIT_TEST(Notes_Smoke)
{
  auto const fileName = "notes.xml";
  auto const fullFileName = my::JoinFoldersToPath({GetPlatform().WritableDir()}, fileName);
  platform::tests_support::ScopedFile sf(fileName);
  {
    auto const notes = Notes::MakeNotes(fullFileName, true);
    notes->CreateNote(MercatorBounds::ToLatLon({1, 2}), "Some note1");
    notes->CreateNote(MercatorBounds::ToLatLon({2, 2}), "Some note2");
    notes->CreateNote(MercatorBounds::ToLatLon({1, 1}), "Some note3");
  }
  {
    auto const notes = Notes::MakeNotes(fullFileName, true);
    auto const result = notes->GetNotes();
    TEST_EQUAL(result.size(), 3, ());
    std::vector<Note> const expected{{MercatorBounds::ToLatLon({1, 2}), "Some note1"},
                                     {MercatorBounds::ToLatLon({2, 2}), "Some note2"},
                                     {MercatorBounds::ToLatLon({1, 1}), "Some note3"}};

    auto const isEqual = std::equal(
        result.begin(), result.end(), expected.begin(), [](Note const & lhs, Note const & rhs) {
          return lhs.m_point.EqualDxDy(rhs.m_point, Notes::kTolerance);
        });
    TEST(isEqual, ());
  }
}
