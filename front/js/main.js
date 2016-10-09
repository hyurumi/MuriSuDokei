"use strict";

// Variables
var current_clock; // A or B;
var data = {};
data.pi = {};
data.phi = {};

//initial ui settings
$('#clock_a').show();
$('#clock_b').show();
$('#widget').show();

$('#mode_description').html(getCurrentModeDescribable());
$('#widget input').click(function(){
    $('#mode_description').html(getCurrentModeDescribable());
});

//loading data
getCSV("pi",function(key, str){
    var tmp = str.split("\n");
    for(var i=0; i<tmp.length; ++i){
        var column = tmp[i].split(',');
        var time = column[0];
        data[key][time] = { "position" : column[1], "sequence": column[2]};
    }
    getCSV("phi", function(key, str){
        var tmp = str.split("\n");
        for(var i=0; i<tmp.length; ++i){
            var column = tmp[i].split(',');
            var time = column[0];
            data[key][time] = { "position" : column[1], "sequence": column[2]};
        }
        init();
    })
});


function init() {

    $('#clock_a').offset({ top: 16000, left:0 });
    $('#clock_b').offset({ top: 18000, left:0 });
    var all_table_columns = $('#clock_a td');
    all_table_columns.each (function(index, domEle){
        domEle.innerHTML = getNow().sequence[index];
    });
    current_clock = "A";
    prepare(getNow(),getNext());
}

function prepare(now, next) {
    var all_table_columns;
    var clock_next;
    var clock_now;
    if (current_clock === "B") {
        clock_next = $('#clock_a');
        clock_now =$ ('#clock_b');
        all_table_columns = $('#clock_a td');
    }else { 
        clock_next =$ ('#clock_b');
        clock_now =$ ('#clock_a');
        all_table_columns = $('#clock_b td');
    }
    all_table_columns.each (function(index, domEle){
        domEle.innerHTML = next.sequence[index];
    });
    if (now.position < next.position) {
        clock_next.offset({ top: clock_now.offset().top - 2000, left:0 });
    } else {
        clock_next.offset({ top: clock_now.offset().top + 2000, left:0 });
    }
}

function jumpToNext(callback){
    var clock;
    if (current_clock === "B") {
        clock = $('#clock_a');
        current_clock = "A";
    }else { 
        clock = $('#clock_b');
        current_clock = "B";
    }
    $("#time_position").html(getNow().position);
    $("html, body").animate({
        scrollTop: clock.offset().top, 
        scrollLeft: clock.offset().left
    }, {
        duration: 300,
        specialEasing: {
            width: "easeInOutExpo",
            height: "easeInOutExpo"
        }
    }, function(){
        callback();
    });
}

function getCurrentMode (){
    var ans;
    if ($("#pi").prop("checked")) {
        ans = "pi";
    }else if ($("#phi").prop("checked")) {
        ans = "phi";
    }else if ($("#sqrt2").prop("checked"))  {
        ans = "sqrt2";
    } else {
        ans = "sqrt5";
    }
    return ans;
}

function getCurrentModeDescribable (){
    var ans;
    if ($("#pi").prop("checked")) {
        ans = "π";
    }else if ($("#phi").prop("checked")) {
        ans = "φ";
    }else if ($("#sqrt2").prop("checked"))  {
        ans = "√2";
    } else {
        ans = "√5";
    }
    return ans;
}

/**
 * Execution part
 */

$(function(){
    setInterval(function(){
        jumpToNext(prepare(getNow(), getNext()));
    },1000);
});


/**
 * Helpers
 */
function getRandomInt(min, max) {
    return Math.floor( Math.random() * (max - min + 1) ) + min;
}

function getNow() {
    var current_time = moment().format('HHmmss');
    return data[getCurrentMode()][current_time];
}

function getNext() {
    var next_time = moment().add(1,'s').format('HHmmss');
    return data[getCurrentMode()][next_time];
}

function getCSV(key, callback){
    var req = new XMLHttpRequest(); 
    req.open("get", "data/" + key +".csv", true); 
    req.send(null);
	
    req.onload = function(){
	    callback(key, req.responseText);
    }
}