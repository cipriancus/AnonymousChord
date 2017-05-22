$(document).ready(function () {
    var anonymous_flag = false;
    var page_load = 1;

    $.ajax({
        type: "GET",
        url: "/getnodes",
        contentType: 'application/json;charset=UTF-8',
        success: function (result) {
            var ips = result.split(" ");

            for (i = 0; i < ips.length; i++) {
                if (ips[i].length > 0)
                    $('#ul_info').append('<li><a href=\"#\"><span class=\"glyphicon glyphicon-one-fine-dot\"><\/span>' + ips[i] + '<\/a></li>')
            }
        }
    });

    $(function () {
        if (anonymous_flag == false)
            $('#anonymous-toggle').bootstrapToggle('off');
        else $('#anonymous-toggle').bootstrapToggle('on');
    })


    $('#send_request').click(function () {
        var user_input = $('#search_input').val();

        $('#jumbotron_content').replaceWith('<div id="loader"><div class="loader center-block"></div><h1>Searching...</h1></div>');

        $.ajax({
            type: "POST",
            url: "/search",
            data: JSON.stringify(user_input, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                $('#loader').replaceWith(result);
            }
        });
    });

    $('#anonymous-toggle').change(function () {
        if (page_load == 0) {
            anonymous_flag = !anonymous_flag;

            $.ajax({
                type: "POST",
                url: "/setflag",
                data: JSON.stringify(anonymous_flag, null, '\t'),
                contentType: 'application/json;charset=UTF-8',
                success: function (result) {
                    if (result == 'False') {
                        anonymous_flag = false
                        $('#anonymous_status').replaceWith('<li id=\"anonymous_status\"><a href=\"#\"><span class=\"glyphicon red glyphicon-one-fine-dot\"><\/span>You are not anonymous<\/a><\/li>')
                    } else if (result == 'True') {
                        anonymous_flag = true
                        $('#anonymous_status').replaceWith('<li id=\"anonymous_status\"><a href=\"#\"><span class=\"glyphicon glyphicon-one-fine-dot\"><\/span>You are anonymous<\/a><\/li>')
                    }
                }
            });
        } else {
            page_load = 0;
        }
    });

    $('#send_new_job').click(function () {
        var title = $('#job_title').val();
        var keywords = $('#job_keywords').val();
        var description = $('#job_description').val();
        var has_error = false;

        if (title.length < 3) {
            $('#job_title').css('border-color', 'red');
            has_error=true;
        } else {
            $('#job_title').css('border-color', 'black');
        }

        if (keywords.length < 3) {
            $('#job_keywords').css('border-color', 'red');
            has_error=true;
        } else {
            $('#job_keywords').css('border-color', 'black');
        }

        if (description.length < 20) {
            $('#job_description').css('border-color', 'red');
            has_error=true;
        } else {
            $('#job_description').css('border-color', 'black');
        }

        if(has_error==true)
            return;


        window.location.href = 'jobs';
    });

    $('#job_dropdown').click(function () {
        window.location.href = 'jobs';
    });

    $('#home_button').click(function () {
        window.location.href = '/';
    });

    $('#messanger_dropdown').click(function () {

    });

    $('#add_job_button').click(function () {
        window.location.href = 'addjob';
    });
});
