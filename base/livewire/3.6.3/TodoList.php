<?php

namespace App\Livewire;

use Livewire\Component;

class TodoList extends Component
{
    // TODO 列表数组
    public array $todos = [];
    
    // 新 TODO 的输入内容
    public string $newTodo = '';

    // 添加新的 TODO 项目
    public function addTodo()
    {
        // 验证输入不为空
        if (trim($this->newTodo) === '') {
            return;
        }

        // 添加到数组开头
        array_unshift($this->todos, $this->newTodo);
        
        // 清空输入框
        $this->newTodo = '';
    }

    // 删除指定索引的 TODO 项目
    public function removeTodo(int $index)
    {
        if (isset($this->todos[$index])) {
            array_splice($this->todos, $index, 1);
        }
    }

    public function render()
    {
        return view('livewire.todo-list');
    }
}
