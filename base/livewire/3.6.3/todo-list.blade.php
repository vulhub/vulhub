<div style="max-width: 500px; margin: 40px auto; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;">
    <h1 style="text-align: center; color: #2d3748; margin-bottom: 30px;">📝 TODO List</h1>
    
    {{-- Add new TODO form --}}
    <form wire:submit="addTodo" style="display: flex; gap: 10px; margin-bottom: 20px;">
        <input 
            type="text" 
            wire:model="newTodo" 
            placeholder="Enter a new task..."
            style="flex: 1; padding: 12px 16px; border: 2px solid #e2e8f0; border-radius: 8px; font-size: 16px; outline: none; transition: border-color 0.2s;"
            onfocus="this.style.borderColor='#667eea'" 
            onblur="this.style.borderColor='#e2e8f0'"
        >
        <button 
            type="submit"
            style="padding: 12px 24px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; border-radius: 8px; font-size: 16px; cursor: pointer; font-weight: 600; transition: transform 0.1s, box-shadow 0.2s;"
            onmouseover="this.style.transform='scale(1.05)'; this.style.boxShadow='0 4px 15px rgba(102, 126, 234, 0.4)'"
            onmouseout="this.style.transform='scale(1)'; this.style.boxShadow='none'"
        >
            Add
        </button>
    </form>

    {{-- TODO list --}}
    @if(count($todos) > 0)
        <ul style="list-style: none; padding: 0; margin: 0;">
            @foreach($todos as $index => $todo)
                <li 
                    style="display: flex; align-items: center; justify-content: space-between; padding: 16px; margin-bottom: 10px; background: #f7fafc; border-radius: 8px; border-left: 4px solid #667eea; transition: transform 0.2s, box-shadow 0.2s;"
                    onmouseover="this.style.transform='translateX(5px)'; this.style.boxShadow='0 2px 10px rgba(0,0,0,0.1)'"
                    onmouseout="this.style.transform='translateX(0)'; this.style.boxShadow='none'"
                >
                    <span style="color: #4a5568; font-size: 16px;">{{ $todo }}</span>
                    <button 
                        wire:click="removeTodo({{ $index }})"
                        style="padding: 6px 12px; background: #fc8181; color: white; border: none; border-radius: 6px; cursor: pointer; font-size: 14px; transition: background 0.2s;"
                        onmouseover="this.style.background='#f56565'"
                        onmouseout="this.style.background='#fc8181'"
                    >
                        Delete
                    </button>
                </li>
            @endforeach
        </ul>
        <p style="text-align: center; color: #a0aec0; margin-top: 20px; font-size: 14px;">
            {{ count($todos) }} {{ count($todos) === 1 ? 'task' : 'tasks' }} total
        </p>
    @else
        <div style="text-align: center; padding: 40px; color: #a0aec0;">
            <p style="font-size: 48px; margin-bottom: 10px;">🎉</p>
            <p style="font-size: 16px;">No tasks yet. Add one to get started!</p>
        </div>
    @endif
</div>
