/**
 * Created by "Dominic Rout" on 03/04/2017.
 */
$(function () {
    var organicField = $("#isOrganic").parent()

    organicField.attr("title", $("#organic_footnote").text())
    organicField.attr("data-toggle", "tooltip")
    $('[data-toggle="tooltip"]').tooltip()
})