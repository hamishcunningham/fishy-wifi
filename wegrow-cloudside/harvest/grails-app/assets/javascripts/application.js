// This is a manifest file that'll be compiled into application.js.
//
// Any JavaScript file within this directory can be referenced here using a relative path.
//
// You're free to add application-wide JavaScript to this file, but it's generally better
// to create separate JavaScript files as needed.
//
//= require jquery-2.2.0.min
//= require bootstrap
//= require_tree .
//= require_self

if (typeof jQuery !== 'undefined') {
    (function($) {
        $(document).ajaxStart(function() {
            $('#spinner').fadeIn();
        }).ajaxStop(function() {
            $('#spinner').fadeOut();
        });
        $('.stacktable').stacktable({myClass:'stacktable'});

        $('[data-toggle="tooltip"]').tooltip()


        $('.harvestHeadingRow').each(function (idx, count) {
            $($(count).data("target")).on("hidden.bs.collapse", function() {
                $(count).find(".harvest-count > .glyphicon")
                    .removeClass("glyphicon-chevron-down")
                    .addClass("glyphicon-chevron-right");
                $(count).removeClass("in");
            });

            $($(count).data("target")).on("shown.bs.collapse", function() {
                $(count).find(".harvest-count > .glyphicon")
                    .removeClass("glyphicon-chevron-right")
                    .addClass("glyphicon-chevron-down");
                $(count).addClass("in");

            });

        });

    })(jQuery);
}


