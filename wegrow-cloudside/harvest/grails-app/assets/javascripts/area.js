$(function() {
    var updateSelectedCrop = function (cropId) {
        if (cropId) {
            $.getJSON("/crop/show.json", {id:cropId}, hideOrShowAreaFields);
        }
    }
    var hideOrShowAreaFields = function (cropData) {
        if (cropData.isTree) {
            $(".form-group:has(input#area)").hide();
            $(".form-group:has(input#area)").val("");
            $(".form-group:has(input#canopyRadius)").show();
        } else {
            $(".form-group:has(input#area)").show();
            $(".form-group:has(input#canopyRadius)").val("");
            $(".form-group:has(input#canopyRadius)").hide();
        }
        if (cropData.isGreenhouseable) {
            $(".checkbox:has(input#inGreenhouse)").show();
        } else {
            $(".form-group:has(input#inGreenhouse)").prop("checked", false);
            $(".checkbox:has(input#inGreenhouse)").hide();
        }
    }

    updateSelectedCrop($("select#crop").val());

    $("select#crop").change(function(event) {
        updateSelectedCrop($(event.target).val());
    })
});