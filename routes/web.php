<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\LandingController;
use App\Http\Controllers\QueryController;

Route::get('/', function () {
    return view('landing');
});

Route::get('/search', [LandingController::class, 'search'])->name('search');
