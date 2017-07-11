$(document).ready(function () {

    function getParameterByName(name, url) {
        if (!url) url = window.location.href;
        name = name.replace(/[\[\]]/g, "\\$&");
        var regex = new RegExp("[?&]" + name + "(=([^&#]*)|&|#|$)"),
            results = regex.exec(url);
        if (!results) return null;
        if (!results[2]) return '';
        return decodeURIComponent(results[2].replace(/\+/g, " "));
    }

    var anonymous_flag = false;

    $.ajax({
            type: "GET",
            url: "/getAnonymousFlag",
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                if (result == 'False') {
                    anonymous_flag=false;
                    $('#anonymous-toggle').bootstrapToggle('off');
                    $('#anonymous_status').replaceWith('<li id=\"anonymous_status\"><a href=\"#\"><span class=\"glyphicon red glyphicon-one-fine-dot\"><\/span>You are not anonymous<\/a><\/li>')

                }else if(result == 'True'){
                    anonymous_flag=true;
                    $('#anonymous-toggle').bootstrapToggle('on');
                    $('#anonymous_status').replaceWith('<li id=\"anonymous_status\"><a href=\"#\"><span class=\"glyphicon glyphicon-one-fine-dot\"><\/span>You are anonymous<\/a><\/li>')

                }
            }
        });

    var page_load = 1;

    if (window.location)
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

    if (top.location.pathname == '/jobs') {
        $.ajax({
            type: "GET",
            url: "/getjobs",
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                var entry_no = 1;
                var index;

                result = result.split('~');

                for (index = 0; index < result.length; ++index) {
                    if (result[index].length > 0) {
                        var splits = result[index].split('#');
                        var id = splits[0];
                        var title = splits[1];
                        $('#list_jobs').append('<div id=\"job\" class=\"col-md-12\" data-id=\"' + id + '\"><h4 class=\"col-md-10\" >( no. ' + entry_no + ' ) ' + title + '<\/h4><a id=\"edit\" href=\"#\" class=\"col-md-1 edit_button\">Edit<\/a><a id=\"delete\" href=\"#\" class=\"col-md-1 delete_button\" >Delete<\/a><\/div>');
                        entry_no++;
                    }
                }
                edit_job_edit_delete_click();
            }
        });
    }

    if (top.location.pathname == '/editjob') {
        var id = getParameterByName('id');

        $.ajax({
            type: "POST",
            url: "/getJobById",
            data: JSON.stringify(id, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                var splits = result.split('#');

                var id = splits[0];
                var title = splits[1];
                var keywords = splits[2];
                var description = splits[3];

                $('#job_title').val(title)
                $('#job_keywords').val(keywords);
                $('#job_description').val(description);
            }
        });

        $('#send_edit_job').click(function () {
            var id = getParameterByName('id');
            var title = $('#job_title').val();
            var keywords = $('#job_keywords').val();
            var description = $('#job_description').val();
            var has_error = false;

            if (title.length < 3 && title.length > 500) {
                $('#job_title').css('border-color', 'red');
                has_error = true;
            } else {
                $('#job_title').css('border-color', 'black');
            }

            if (keywords.length < 3 && keywords.length > 500) {
                $('#job_keywords').css('border-color', 'red');
                has_error = true;
            } else {
                $('#job_keywords').css('border-color', 'black');
            }

            if (description.length < 20 && description.length > 5000) {
                $('#job_description').css('border-color', 'red');
                has_error = true;
            } else {
                $('#job_description').css('border-color', 'black');
            }

            if (has_error == true)
                return;

            var data = {'id': id, 'title': title, 'keywords': keywords, 'description': description};

            $.ajax({
                type: "POST",
                url: "/submiteditjob",
                data: JSON.stringify(data),
                contentType: 'application/json;charset=UTF-8',
                success: function (result) {
                    if (result == 'ok')
                        window.location.href = 'jobs';
                }
            });
        });
    }

    if (top.location.pathname == '/viewjob') {
        var id = getParameterByName('id');

        $.ajax({
            type: "POST",
            url: "/getJobById",
            data: JSON.stringify(id, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                var splits = result.split('#');

                var id = splits[0];
                var title = splits[1];
                var keywords = splits[2];
                var description = splits[3];

                $('#job_title').val(title)
                $('#job_keywords').val(keywords);
                $('#job_description').val(description);
            }
        });
    }

    if (top.location.pathname == '/history') {
        $.ajax({
            type: "GET",
            url: "/getHistory",
            data: JSON.stringify(id, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                var entry_no = 1;
                var index;

                result = result.split('~');

                if(result.length==1){
                        $('#list_jobs').append('<div class=\"job\" class=\"col-md-12\"><h4>Sorry, no jobs in history<\/h4><\/div>');
                }

                for (index = 0; index < result.length; ++index) {
                    if (result[index].length > 0) {
                        var splits = result[index].split('#');
                        var id = splits[0];
                        var title = splits[1];
                        $('#list_jobs').append('<div class=\"job\" class=\"col-md-12\" data-id=\"' + id + '\"><h4><a class=\"result_job\" href=\"#\" class=\"edit_button\">( no. ' + entry_no + ' ) ' + title + '<\/a><\/h4><\/div>');
                        entry_no++;
                    }
                }
                view_job_click();
            }
        });
    }

    $('#send_request').click(function () {
        var user_input = $('#search_input').val();

        $('#jumbotron_content').replaceWith('<div id="loader"><div class="loader center-block"></div><h1>Searching...</h1></div>');

        $.ajax({
            type: "POST",
            url: "/search",
            data: JSON.stringify(user_input, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                $('#loader').replaceWith('<div class=\"job_class left_text\" id=\"job_window\"><div class=\"job_inner_class\"><h2>Results:</h2><div class=\"job_list\" id=\"list_jobs\"><\/div><\/div><\/div>');

                var entry_no = 1;
                var index;

                if(result == '0'){
                    $('#list_jobs').append('<div class=\"job col-md-12\"><h4>No results found<\/h4><\/div>');
                    return;
                }

                result = result.split('~');

                for (index = 0; index < result.length; ++index) {
                    if (result[index].length > 0) {
                        var splits = result[index].split('#');
                        var id = splits[0];
                        var title = splits[1];
                        var keywords = splits[2];
                        var description = splits[3];

                        $('#list_jobs').append('<div class=\"job col-md-12\" data-id=\"' + id + '\"><h4><a class=\"result_job col-md-8\" href=\"#\">( no. ' + entry_no + ' ) ' + title + '<\/a><a class=\"col-md-4\" style="text-decoration: none;">'+keywords+'<\/a><\/h4><\/div>');
                        entry_no++;
                    }
                }
                view_job_click();
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

        if (title.length < 3 && title.length > 500) {
            $('#job_title').css('border-color', 'red');
            has_error = true;
        } else {
            $('#job_title').css('border-color', 'black');
        }

        if (keywords.length < 3 && keywords.length > 500) {
            $('#job_keywords').css('border-color', 'red');
            has_error = true;
        } else {
            $('#job_keywords').css('border-color', 'black');
        }

        if (description.length < 20 && description.length > 5000) {
            $('#job_description').css('border-color', 'red');
            has_error = true;
        } else {
            $('#job_description').css('border-color', 'black');
        }

        if (has_error == true)
            return;

        var data = {'title': title, 'keywords': keywords, 'description': description};

        $.ajax({
            type: "POST",
            url: "/addnewjob",
            data: JSON.stringify(data),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                if (result == 'ok')
                    window.location.href = 'jobs';
            }
        });


    });

    $('#job_dropdown').click(function () {
        window.location.href = 'jobs';
    });

    $('#home_button').click(function () {
        window.location.href = '/';
    });

    $('#history_dropdown').click(function () {
        window.location.href = 'history';
    });

    $('#add_job_button').click(function () {
        window.location.href = 'addjob';
    });
});

function view_job_click() {
    $('.result_job').click(function () {
        var id = $(this).closest('div').data('id');

        window.open('viewjob?id=' + id);
    });
}

function edit_job_edit_delete_click() {
    $('.edit_button').click(function () {
        var id = $(this).closest('div').data('id');

        window.location.href = 'editjob?id=' + id;
    });

    $('.delete_button').click(function () {
        var id = $(this).closest('div').data('id');

        $.ajax({
            type: "POST",
            url: "/deletejob",
            data: JSON.stringify(id, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function (result) {
                if (result == 'ok') {
                    location.reload();
                }
            }
        });
    });
}
