/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#include "minddata/dataset/include/transforms.h"
#include "minddata/dataset/kernels/image/image_utils.h"

#include "minddata/dataset/kernels/image/center_crop_op.h"
#include "minddata/dataset/kernels/image/cut_out_op.h"
#include "minddata/dataset/kernels/image/decode_op.h"
#include "minddata/dataset/kernels/image/normalize_op.h"
#include "minddata/dataset/kernels/image/pad_op.h"
#include "minddata/dataset/kernels/image/random_color_adjust_op.h"
#include "minddata/dataset/kernels/image/random_crop_op.h"
#include "minddata/dataset/kernels/image/random_horizontal_flip_op.h"
#include "minddata/dataset/kernels/image/random_rotation_op.h"
#include "minddata/dataset/kernels/image/random_vertical_flip_op.h"
#include "minddata/dataset/kernels/image/resize_op.h"
#include "minddata/dataset/kernels/image/uniform_aug_op.h"

namespace mindspore {
namespace dataset {
namespace api {

TensorOperation::TensorOperation() {}

// Transform operations for computer vision.
namespace vision {

// Function to create CenterCropOperation.
std::shared_ptr<CenterCropOperation> CenterCrop(std::vector<int32_t> size) {
  auto op = std::make_shared<CenterCropOperation>(size);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create CutOutOp.
std::shared_ptr<CutOutOperation> CutOut(int32_t length, int32_t num_patches) {
  auto op = std::make_shared<CutOutOperation>(length, num_patches);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create DecodeOperation.
std::shared_ptr<DecodeOperation> Decode(bool rgb) {
  auto op = std::make_shared<DecodeOperation>(rgb);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create NormalizeOperation.
std::shared_ptr<NormalizeOperation> Normalize(std::vector<float> mean, std::vector<float> std) {
  auto op = std::make_shared<NormalizeOperation>(mean, std);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create PadOperation.
std::shared_ptr<PadOperation> Pad(std::vector<int32_t> padding, std::vector<uint8_t> fill_value,
                                  BorderType padding_mode) {
  auto op = std::make_shared<PadOperation>(padding, fill_value, padding_mode);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create RandomColorAdjustOperation.
std::shared_ptr<RandomColorAdjustOperation> RandomColorAdjust(std::vector<float> brightness,
                                                              std::vector<float> contrast,
                                                              std::vector<float> saturation, std::vector<float> hue) {
  auto op = std::make_shared<RandomColorAdjustOperation>(brightness, contrast, saturation, hue);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create RandomCropOperation.
std::shared_ptr<RandomCropOperation> RandomCrop(std::vector<int32_t> size, std::vector<int32_t> padding,
                                                bool pad_if_needed, std::vector<uint8_t> fill_value) {
  auto op = std::make_shared<RandomCropOperation>(size, padding, pad_if_needed, fill_value);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create RandomHorizontalFlipOperation.
std::shared_ptr<RandomHorizontalFlipOperation> RandomHorizontalFlip(float prob) {
  auto op = std::make_shared<RandomHorizontalFlipOperation>(prob);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create RandomRotationOperation.
std::shared_ptr<RandomRotationOperation> RandomRotation(std::vector<float> degrees, InterpolationMode resample,
                                                        bool expand, std::vector<float> center,
                                                        std::vector<uint8_t> fill_value) {
  auto op = std::make_shared<RandomRotationOperation>(degrees, resample, expand, center, fill_value);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create RandomVerticalFlipOperation.
std::shared_ptr<RandomVerticalFlipOperation> RandomVerticalFlip(float prob) {
  auto op = std::make_shared<RandomVerticalFlipOperation>(prob);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create ResizeOperation.
std::shared_ptr<ResizeOperation> Resize(std::vector<int32_t> size, InterpolationMode interpolation) {
  auto op = std::make_shared<ResizeOperation>(size, interpolation);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

// Function to create UniformAugOperation.
std::shared_ptr<UniformAugOperation> UniformAugment(std::vector<std::shared_ptr<TensorOperation>> transforms,
                                                    int32_t num_ops) {
  auto op = std::make_shared<UniformAugOperation>(transforms, num_ops);
  // Input validation
  if (!op->ValidateParams()) {
    return nullptr;
  }
  return op;
}

/* ####################################### Derived TensorOperation classes ################################# */

// CenterCropOperation
CenterCropOperation::CenterCropOperation(std::vector<int32_t> size) : size_(size) {}

bool CenterCropOperation::ValidateParams() {
  if (size_.empty() || size_.size() > 2) {
    MS_LOG(ERROR) << "CenterCrop: size vector has incorrect size.";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> CenterCropOperation::Build() {
  int32_t crop_height = size_[0];
  int32_t crop_width = 0;

  // User has specified crop_width.
  if (size_.size() == 2) {
    crop_width = size_[1];
  }

  std::shared_ptr<CenterCropOp> tensor_op = std::make_shared<CenterCropOp>(crop_height, crop_width);
  return tensor_op;
}

// CutOutOperation
CutOutOperation::CutOutOperation(int32_t length, int32_t num_patches) : length_(length), num_patches_(num_patches) {}

bool CutOutOperation::ValidateParams() {
  if (length_ < 0) {
    MS_LOG(ERROR) << "CutOut: length cannot be negative";
    return false;
  }
  if (num_patches_ < 0) {
    MS_LOG(ERROR) << "CutOut: number of patches cannot be negative";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> CutOutOperation::Build() {
  std::shared_ptr<CutOutOp> tensor_op = std::make_shared<CutOutOp>(length_, length_, num_patches_, false, 0, 0, 0);
  return tensor_op;
}

// DecodeOperation
DecodeOperation::DecodeOperation(bool rgb) : rgb_(rgb) {}

bool DecodeOperation::ValidateParams() { return true; }

std::shared_ptr<TensorOp> DecodeOperation::Build() { return std::make_shared<DecodeOp>(rgb_); }

// NormalizeOperation
NormalizeOperation::NormalizeOperation(std::vector<float> mean, std::vector<float> std) : mean_(mean), std_(std) {}

bool NormalizeOperation::ValidateParams() {
  if (mean_.size() != 3) {
    MS_LOG(ERROR) << "Normalize: mean vector has incorrect size: " << mean_.size();
    return false;
  }

  if (std_.size() != 3) {
    MS_LOG(ERROR) << "Normalize: std vector has incorrect size: " << std_.size();
    return false;
  }

  return true;
}

std::shared_ptr<TensorOp> NormalizeOperation::Build() {
  return std::make_shared<NormalizeOp>(mean_[0], mean_[1], mean_[2], std_[0], std_[1], std_[2]);
}

// PadOperation
PadOperation::PadOperation(std::vector<int32_t> padding, std::vector<uint8_t> fill_value, BorderType padding_mode)
    : padding_(padding), fill_value_(fill_value), padding_mode_(padding_mode) {}

bool PadOperation::ValidateParams() {
  if (padding_.empty() || padding_.size() == 3 || padding_.size() > 4) {
    MS_LOG(ERROR) << "Pad: padding vector has incorrect size: padding.size()";
    return false;
  }

  if (fill_value_.empty() || (fill_value_.size() != 1 && fill_value_.size() != 3)) {
    MS_LOG(ERROR) << "Pad: fill_value vector has incorrect size: fill_value.size()";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> PadOperation::Build() {
  int32_t pad_top, pad_bottom, pad_left, pad_right;
  switch (padding_.size()) {
    case 1:
      pad_left = padding_[0];
      pad_top = padding_[0];
      pad_right = padding_[0];
      pad_bottom = padding_[0];
      break;
    case 2:
      pad_left = padding_[0];
      pad_top = padding_[1];
      pad_right = padding_[0];
      pad_bottom = padding_[1];
      break;
    default:
      pad_left = padding_[0];
      pad_top = padding_[1];
      pad_right = padding_[2];
      pad_bottom = padding_[3];
  }
  uint8_t fill_r, fill_g, fill_b;

  fill_r = fill_value_[0];
  fill_g = fill_value_[0];
  fill_b = fill_value_[0];

  if (fill_value_.size() == 3) {
    fill_r = fill_value_[0];
    fill_g = fill_value_[1];
    fill_b = fill_value_[2];
  }

  std::shared_ptr<PadOp> tensor_op =
    std::make_shared<PadOp>(pad_top, pad_bottom, pad_left, pad_right, padding_mode_, fill_r, fill_g, fill_b);
  return tensor_op;
}

// RandomColorAdjustOperation.
RandomColorAdjustOperation::RandomColorAdjustOperation(std::vector<float> brightness, std::vector<float> contrast,
                                                       std::vector<float> saturation, std::vector<float> hue)
    : brightness_(brightness), contrast_(contrast), saturation_(saturation), hue_(hue) {}

bool RandomColorAdjustOperation::ValidateParams() {
  // Do some input validation.
  if (brightness_.empty() || brightness_.size() > 2) {
    MS_LOG(ERROR) << "RandomColorAdjust: brightness must be a vector of one or two values";
    return false;
  }
  if (contrast_.empty() || contrast_.size() > 2) {
    MS_LOG(ERROR) << "RandomColorAdjust: contrast must be a vector of one or two values";
    return false;
  }
  if (saturation_.empty() || saturation_.size() > 2) {
    MS_LOG(ERROR) << "RandomColorAdjust: saturation must be a vector of one or two values";
    return false;
  }
  if (hue_.empty() || hue_.size() > 2) {
    MS_LOG(ERROR) << "RandomColorAdjust: hue must be a vector of one or two values";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> RandomColorAdjustOperation::Build() {
  float brightness_lb, brightness_ub, contrast_lb, contrast_ub, saturation_lb, saturation_ub, hue_lb, hue_ub;

  brightness_lb = brightness_[0];
  brightness_ub = brightness_[0];

  if (brightness_.size() == 2) brightness_ub = brightness_[1];

  contrast_lb = contrast_[0];
  contrast_ub = contrast_[0];

  if (contrast_.size() == 2) contrast_ub = contrast_[1];

  saturation_lb = saturation_[0];
  saturation_ub = saturation_[0];

  if (saturation_.size() == 2) saturation_ub = saturation_[1];

  hue_lb = hue_[0];
  hue_ub = hue_[0];

  if (hue_.size() == 2) hue_ub = hue_[1];

  std::shared_ptr<RandomColorAdjustOp> tensor_op = std::make_shared<RandomColorAdjustOp>(
    brightness_lb, brightness_ub, contrast_lb, contrast_ub, saturation_lb, saturation_ub, hue_lb, hue_ub);
  return tensor_op;
}

// RandomCropOperation
RandomCropOperation::RandomCropOperation(std::vector<int32_t> size, std::vector<int32_t> padding, bool pad_if_needed,
                                         std::vector<uint8_t> fill_value)
    : size_(size), padding_(padding), pad_if_needed_(pad_if_needed), fill_value_(fill_value) {}

bool RandomCropOperation::ValidateParams() {
  if (size_.empty() || size_.size() > 2) {
    MS_LOG(ERROR) << "RandomCrop: size vector has incorrect size: " << size_.size();
    return false;
  }

  if (padding_.empty() || padding_.size() != 4) {
    MS_LOG(ERROR) << "RandomCrop: padding vector has incorrect size: padding.size()";
    return false;
  }

  if (fill_value_.empty() || fill_value_.size() != 3) {
    MS_LOG(ERROR) << "RandomCrop: fill_value vector has incorrect size: fill_value.size()";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> RandomCropOperation::Build() {
  int32_t crop_height = size_[0];
  int32_t crop_width = 0;

  int32_t pad_top = padding_[0];
  int32_t pad_bottom = padding_[1];
  int32_t pad_left = padding_[2];
  int32_t pad_right = padding_[3];

  uint8_t fill_r = fill_value_[0];
  uint8_t fill_g = fill_value_[1];
  uint8_t fill_b = fill_value_[2];

  // User has specified the crop_width value.
  if (size_.size() == 2) {
    crop_width = size_[1];
  }

  auto tensor_op = std::make_shared<RandomCropOp>(crop_height, crop_width, pad_top, pad_bottom, pad_left, pad_right,
                                                  BorderType::kConstant, pad_if_needed_, fill_r, fill_g, fill_b);
  return tensor_op;
}

// RandomHorizontalFlipOperation
RandomHorizontalFlipOperation::RandomHorizontalFlipOperation(float probability) : probability_(probability) {}

bool RandomHorizontalFlipOperation::ValidateParams() { return true; }

std::shared_ptr<TensorOp> RandomHorizontalFlipOperation::Build() {
  std::shared_ptr<RandomHorizontalFlipOp> tensor_op = std::make_shared<RandomHorizontalFlipOp>(probability_);
  return tensor_op;
}

// Function to create RandomRotationOperation.
RandomRotationOperation::RandomRotationOperation(std::vector<float> degrees, InterpolationMode interpolation_mode,
                                                 bool expand, std::vector<float> center,
                                                 std::vector<uint8_t> fill_value)
    : degrees_(degrees),
      interpolation_mode_(interpolation_mode),
      expand_(expand),
      center_(center),
      fill_value_(fill_value) {}

bool RandomRotationOperation::ValidateParams() {
  if (degrees_.empty() || degrees_.size() != 2) {
    MS_LOG(ERROR) << "RandomRotation: degrees vector has incorrect size: degrees.size()";
    return false;
  }
  if (center_.empty() || center_.size() != 2) {
    MS_LOG(ERROR) << "RandomRotation: center vector has incorrect size: center.size()";
    return false;
  }
  if (fill_value_.empty() || fill_value_.size() != 3) {
    MS_LOG(ERROR) << "RandomRotation: fill_value vector has incorrect size: fill_value.size()";
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> RandomRotationOperation::Build() {
  std::shared_ptr<RandomRotationOp> tensor_op =
    std::make_shared<RandomRotationOp>(degrees_[0], degrees_[1], center_[0], center_[1], interpolation_mode_, expand_,
                                       fill_value_[0], fill_value_[1], fill_value_[2]);
  return tensor_op;
}

// RandomVerticalFlipOperation
RandomVerticalFlipOperation::RandomVerticalFlipOperation(float probability) : probability_(probability) {}

bool RandomVerticalFlipOperation::ValidateParams() { return true; }

std::shared_ptr<TensorOp> RandomVerticalFlipOperation::Build() {
  std::shared_ptr<RandomVerticalFlipOp> tensor_op = std::make_shared<RandomVerticalFlipOp>(probability_);
  return tensor_op;
}

// ResizeOperation
ResizeOperation::ResizeOperation(std::vector<int32_t> size, InterpolationMode interpolation)
    : size_(size), interpolation_(interpolation) {}

bool ResizeOperation::ValidateParams() {
  if (size_.empty() || size_.size() > 2) {
    MS_LOG(ERROR) << "Resize: size vector has incorrect size: " << size_.size();
    return false;
  }
  return true;
}

std::shared_ptr<TensorOp> ResizeOperation::Build() {
  int32_t height = size_[0];
  int32_t width = 0;

  // User specified the width value.
  if (size_.size() == 2) {
    width = size_[1];
  }

  return std::make_shared<ResizeOp>(height, width, interpolation_);
}

// UniformAugOperation
UniformAugOperation::UniformAugOperation(std::vector<std::shared_ptr<TensorOperation>> transforms, int32_t num_ops)
    : transforms_(transforms), num_ops_(num_ops) {}

bool UniformAugOperation::ValidateParams() { return true; }

std::shared_ptr<TensorOp> UniformAugOperation::Build() {
  std::vector<std::shared_ptr<TensorOp>> tensor_ops;
  (void)std::transform(transforms_.begin(), transforms_.end(), std::back_inserter(tensor_ops),
                       [](std::shared_ptr<TensorOperation> op) -> std::shared_ptr<TensorOp> { return op->Build(); });
  std::shared_ptr<UniformAugOp> tensor_op = std::make_shared<UniformAugOp>(tensor_ops, num_ops_);
  return tensor_op;
}

}  // namespace vision
}  // namespace api
}  // namespace dataset
}  // namespace mindspore
