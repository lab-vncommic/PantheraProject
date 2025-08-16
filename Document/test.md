# Ví dụ về ngắt trang trong Markdown

# 🏠 Trang 1 - Giới thiệu

Chào mừng bạn đến với tài liệu hướng dẫn về **ngắt trang trong Markdown**! Đây là một kỹ thuật rất hữu ích khi bạn muốn tạo ra các tài liệu PDF có cấu trúc rõ ràng và chuyên nghiệp.

## 📖 Mục đích của tài liệu

Tài liệu này sẽ hướng dẫn bạn:
- Cách tạo ngắt trang trong Markdown
- Sử dụng CSS để tạo khung viền đẹp mắt  
- Xuất file thành PDF với định dạng chuyên nghiệp
- Áp dụng các kỹ thuật styling nâng cao

## 🎯 Đối tượng sử dụng

- **Sinh viên** viết báo cáo, luận văn
- **Kỹ sư** tạo tài liệu kỹ thuật
- **Nhà phát triển** viết documentation
- **Bất kỳ ai** muốn tạo PDF đẹp từ Markdown

## 📋 Một số thông tin quan trọng

- 🔹 **Điểm 1**: Markdown hỗ trợ HTML và CSS inline
- 🔹 **Điểm 2**: Page break chỉ hoạt động khi export PDF  
- 🔹 **Điểm 3**: Có thể tùy chỉnh styling theo ý muốn

## 💡 Lợi ích của việc sử dụng khung viền

**Khung viền** giúp tài liệu trở nên:
- ✅ **Chuyên nghiệp** hơn trong mắt người đọc
- ✅ **Dễ đọc** với cấu trúc rõ ràng
- ✅ **Nổi bật** so với tài liệu thông thường
- ✅ **Có thể in** với chất lượng cao

**Trang 1 của 3 - Tài liệu hướng dẫn ngắt trang Markdown**

---

<div class="page-break"></div>

# ⚙️ Trang 2 - Chi tiết kỹ thuật

Trang này sẽ trình bày các **phương pháp kỹ thuật** để thực hiện ngắt trang và styling trong Markdown. Đây là những kiến thức nâng cao giúp bạn tạo ra những tài liệu chuyên nghiệp.

## 🔧 Các công cụ cần thiết

**1. Extension VS Code:**
- `Markdown PDF` - Export sang PDF
- `Markdown Preview Enhanced` - Preview nâng cao
- `Markdown All in One` - Công cụ hỗ trợ toàn diện

**2. Phần mềm bên ngoài:**
- `Pandoc` - Chuyển đổi document mạnh mẽ
- `Prince CSS` - PDF generator chuyên nghiệp
- `WeasyPrint` - HTML/CSS to PDF

## 📊 Bảng thông số kỹ thuật

| Phương pháp | Độ phức tạp | Tính năng | Khuyến nghị |
|-------------|-------------|-----------|-------------|
| CSS inline  | ⭐⭐ Dễ     | Cơ bản    | ✅ Nên dùng |
| External CSS| ⭐⭐⭐ Trung bình | Nâng cao | 🔶 Tùy chọn |
| Pandoc      | ⭐⭐⭐⭐ Khó | Chuyên nghiệp | 🚀 Cao cấp |

## 💻 Code example - CSS nâng cao

```css
/* CSS cho ngắt trang nâng cao */
@page {
    margin: 2cm;
    border: 2px solid #333;
    
    @top-center {
        content: "Tài liệu kỹ thuật";
        font-size: 12pt;
    }
}

.page-break {
    page-break-after: always;
    page-break-inside: avoid;
}
```

## 🎨 Styling Tips

**Màu sắc phổ biến:**
- `#2c3e50` - Xanh đậm (professional)
- `#3498db` - Xanh dương (friendly)  
- `#e74c3c` - Đỏ (warning/important)
- `#27ae60` - Xanh lá (success/positive)

## 🧪 Ví dụ thực tế - Hello World

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=================================\n");
    printf("    CHƯƠNG TRÌNH HELLO WORLD     \n");
    printf("=================================\n");
    printf("Hello World from Markdown PDF!\n");
    printf("Tài liệu được tạo bởi: Markdown\n");
    printf("Export tool: VS Code Extension\n");
    printf("=================================\n");
    
    return 0;
}
```

**Trang 2 của 3 - Tài liệu hướng dẫn ngắt trang Markdown**

---

<div class="page-break"></div>
<pagebreak></pagebreak>

# 📋 Trang 3 - Kết luận & Hướng dẫn thực hành

Chúc mừng bạn đã hoàn thành việc tìm hiểu về **ngắt trang trong Markdown**! Trang cuối này sẽ tổng kết kiến thức và đưa ra hướng dẫn thực hành.

## 🎯 Tóm tắt kiến thức đã học

**Chúng ta đã tìm hiểu:**
1. ✅ Cách tạo khung viền cho trang PDF
2. ✅ Sử dụng CSS inline để styling  
3. ✅ Các phương pháp ngắt trang khác nhau
4. ✅ Best practices cho typography và màu sắc
5. ✅ Tools và extensions hỗ trợ

## 🛠️ Các phương pháp ngắt trang trong Markdown:

**1. 🎨 CSS Page Break After:**
```html
<div style="page-break-after: always;"></div>
```
*Sử dụng khi muốn ngắt trang sau element hiện tại*

**2. 📄 CSS Page Break Before:**
```html
<div style="page-break-before: always;"></div>
```
*Sử dụng khi muốn ngắt trang trước element hiện tại*

**3. 🏷️ Custom HTML Tag:**
```html
<pagebreak></pagebreak>
```
*Cần định nghĩa CSS riêng, linh hoạt hơn*

## 📚 Bài tập thực hành

**Nhiệm vụ cho bạn:**
1. 📝 Tạo một tài liệu 5 trang với nội dung tự do
2. 🎨 Áp dụng ít nhất 2 màu khung khác nhau
3. 📊 Thêm ít nhất 1 bảng và 1 đoạn code
4. 🖼️ Sử dụng emoji và typography đẹp mắt
5. 📄 Export thành PDF và kiểm tra kết quả

## ⚠️ Lưu ý quan trọng khi thực hành

- ✅ **Ngắt trang** chỉ có hiệu lực khi xuất ra PDF
- 🔧 **CSS styling** có thể cần điều chỉnh theo từng tool
- 👀 **Preview** luôn trước khi in để đảm bảo layout đúng
- 🎯 **Khung viền** giúp tài liệu chuyên nghiệp hơn
- 💡 **Thử nghiệm** với các màu sắc và font chữ khác nhau

## 🚀 Bước tiếp theo

**Để trở thành expert:**
1. 📖 Tìm hiểu thêm về Pandoc và LaTeX
2. 🎨 Học CSS nâng cao cho print media
3. 🔧 Thử nghiệm với các template có sẵn
4. 👥 Chia sẻ kinh nghiệm với cộng đồng
5. 🏆 Áp dụng vào dự án thực tế

---

**🎉 Kết thúc tài liệu 🎉**

*Cảm ơn bạn đã đọc! Document được tạo với ❤️*

**Trang 3 của 3 - Tài liệu hướng dẫn ngắt trang Markdown**
}