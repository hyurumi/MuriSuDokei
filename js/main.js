"use strict";

// Variables
var current_clock; // A or B;
var data = {
    pi: {}, phi: {}, sqrt2:{}, sqrt5:{}
};
var left = 0;
const TABLE_SIZE = 804;

/**
 * Initialization
 */
var window_width = $(window).width();
left = (window_width > TABLE_SIZE) ? (window_width - TABLE_SIZE) / 2 : 0;
$(window).resize(function() {
    var window_width = $(window).width();
    left = (window_width > TABLE_SIZE) ? (window_width - TABLE_SIZE) / 2 : 0;
});

$('#clock_a').offset({ top: 16000, left: left});
$('#clock_b').offset({ top: 18000, left: left});
$('#clock_a').show();
$('#clock_b').show();

$('#mode_description').html(getCurrentModeSymbol());
$('#widget input').click(function(){
    $('#mode_description').html(getCurrentModeSymbol());
});

$('#widget').show();

getData("pi",function(key, str){
    var all_table_columns = $('#clock_a td');
    all_table_columns.each (function(index, domEle){
        domEle.innerHTML = getNow().sequence[index];
    });
    current_clock = "A";
    prepare(getNow(),getNext());
});

getData("phi");
getData("sqrt2");

$(function(){
    setInterval(function(){
        jumpToNext(prepare(getNow(), getNext()));
    },1000);
});

/**
 * Routine tasks
 */
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
        clock_next.offset({ top: clock_now.offset().top - 2000, left: left});
    } else {
        clock_next.offset({ top: clock_now.offset().top + 2000, left: left});
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
        scrollLeft: clock.offset().left,
    }, {
        duration: 300,
        specialEasing: {
            scrollTop: "easeInOutExpo",
            scrollLeft: "easeInOutExpo"
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

function getCurrentModeSymbol (){
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

function getData(mode, callback) {
    getCSV(mode, function(key, str){
        var tmp = str.split("\n");
        for(var i=0; i<tmp.length; ++i){
            var column = tmp[i].split(',');
            var time = column[0];
            data[mode][time] = { "position" : column[1], "sequence": column[2]};
        }
        if (callback != null) callback();
    });
}

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

function getCSV(mode, callback){
    var req = new XMLHttpRequest(); 
    req.open("get", "data/" + mode +".csv", true); 
    req.send(null);
	
    req.onload = function(){
	    callback(mode, req.responseText);
    }
}