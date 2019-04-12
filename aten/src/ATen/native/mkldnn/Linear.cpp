#include <ATen/ATen.h>
#include <ATen/Config.h>
#include <ATen/NativeFunctions.h>

#if !AT_MKLDNN_ENABLED()

namespace at {
namespace native {

Tensor mkldnn_linear(
    Tensor& result,
    const Tensor& self,
    const Tensor& mat1,
    const Tensor& mat2,
    Scalar beta,
    Scalar alpha) {
  AT_ERROR("mkldnn_linear: ATen not compiled with MKLDNN support");
}

} // namespace native
} // namespace at

#else // AT_MKLDNN_EBABLED

#include <ATen/native/mkldnn/MKLDNNCommon.h>

namespace at {
namespace native {

Tensor mkldnn_linear(
    const Tensor& input,
    const Tensor& weight,
    const Tensor& bias) {
  ideep::tensor& x = itensor_from_mkldnn(input);
  ideep::tensor& w = itensor_from_mkldnn(weight);

  ideep::tensor y;
  if (bias.defined()) {
    ideep::tensor& b = itensor_from_mkldnn(bias);
    ideep::inner_product_forward::compute(x, w, b, y);
  } else {
    ideep::inner_product_forward::compute(x, w, y);
  }

  return new_with_itensor_mkldnn(std::move(y), input.options());
}

} // namespace native
} // namespace at

#endif // AT_MKLDNN_EBABLED
