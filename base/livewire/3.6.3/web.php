<?php

use Illuminate\Support\Facades\Route;
use App\Livewire\TodoList;

Route::get('/', TodoList::class);
