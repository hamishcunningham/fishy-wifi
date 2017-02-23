$(function() {
    var updateSelectedCrop = function (cropId) {
        if (cropId) {
            $.getJSON("/crop/show.json", {id:cropId}, hideOrShowAreaFields);
        }
    }
    var hideOrShowAreaFields = function (cropData) {
        if (cropData.isTree) {
            $(".form-group:has(input#area)").hide();
            $("input#area").val("");
            $("input#area").removeAttr("required");

            $(".form-group:has(input#canopyRadius)").show();
            $("input#canopyRadius").attr("required", "required");
        } else {
            $(".form-group:has(input#area)").show();
            $("input#canopyRadius").val("");
            $("input#canopyRadius").removeAttr("required");
            $("input#area").attr("required", "required");

            $(".form-group:has(input#canopyRadius)").hide();

        }
        if (cropData.isGreenhouseable) {
            $(".checkbox:has(input#inGreenhouse)").show();
        } else {
            $("input#inGreenhouse").prop("checked", false);
            $(".checkbox:has(input#inGreenhouse)").hide();
        }
    }

    updateSelectedCrop($("select#crop").val());

    $("select#crop").change(function(event) {
        updateSelectedCrop($(event.target).val());
    })
});