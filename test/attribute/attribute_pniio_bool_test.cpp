//
// (c) Copyright 2018 DESY,ESS
//
// This file is part of h5cpp.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor
// Boston, MA  02110-1301 USA
// ===========================================================================
//
// Author: Jan Kotanski <jan,kotanski@desy.de>
// Created on: Jul 2, 2018
//
#include <gtest/gtest.h>
#include <h5cpp/hdf5.hpp>
#include <h5cpp/datatype/enum.hpp>
#include <h5cpp/datatype/ebool.hpp>

using namespace hdf5;

class PNIIOBoolTest : public testing::Test
{
 protected:
  file::File pniio_file;
  node::Group root_group;

  virtual void SetUp()
  {
    pniio_file = file::open("./pniio_test_boolattr.h5", file::AccessFlags::READONLY);
    root_group = pniio_file.root();
  }

};

#ifndef _MSC_VER

TEST_F(PNIIOBoolTest, test_read_simple_bool)
{
  auto attrue = root_group.attributes["bool_true"];
  bool buffer;
  attrue.read(buffer);
  EXPECT_EQ(buffer, true);
  bool buffer2;
  auto atfalse = root_group.attributes["bool_false"];
  atfalse.read(buffer2);
  EXPECT_EQ(buffer2, false);
}

TEST_F(PNIIOBoolTest, test_read_vector_bool)
{
  auto a = root_group.attributes["bool_array"];
  // missing iterator which points to separate bits
  // std::vector<bool> buffer(4);
  // std::vector<bool> ref  = {false, true, true, false};
  // a.read(buffer);
  // EXPECT_EQ(buffer, ref);
  std::vector<unsigned char> buffer2(4);
  std::vector<unsigned char> ref2 = {0, 1, 1, 0};
  EXPECT_EQ(a.datatype().get_class(), datatype::Class::INTEGER);
  EXPECT_EQ(a.datatype().size(), 1ul);
  a.read(buffer2);
  EXPECT_EQ(buffer2, ref2);
}

TEST_F(PNIIOBoolTest, test_read_scalar_ebool)
{
  auto attrue = root_group.attributes["bool_true"];
  datatype::EBool buffer;
  // does not work because of #309, #347
  // attrue.read(buffer);
  attrue.read(buffer, attrue.datatype());
  EXPECT_EQ(buffer, true);
  EXPECT_EQ(buffer, 1);
  EXPECT_EQ(buffer, datatype::EBool::TRUE);
  datatype::EBool buffer2;
  auto atfalse = root_group.attributes["bool_false"];
  // does not work because of #309, #347
  // atfalse.read(buffer2);
  atfalse.read(buffer2, atfalse.datatype());
  EXPECT_EQ(buffer2, false);
  EXPECT_EQ(buffer2, 0);
  EXPECT_EQ(buffer2, datatype::EBool::FALSE);
}

TEST_F(PNIIOBoolTest, test_read_vector_ebool)
{
  auto a = root_group.attributes["bool_array"];
  std::vector<datatype::EBool> buffer(4);
  std::vector<datatype::EBool> eref = {datatype::EBool::FALSE,
                                       datatype::EBool::TRUE,
                                       datatype::EBool::TRUE,
                                       datatype::EBool::FALSE};
  EXPECT_EQ(a.datatype().get_class(), datatype::Class::INTEGER);
  EXPECT_EQ(a.datatype().size(), 1ul);
  // does not work because of #309, #347
  // a.read(buffer);
  a.read(buffer, a.datatype());
  EXPECT_EQ(buffer, eref);
}

#endif
