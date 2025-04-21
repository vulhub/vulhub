from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.core.window import Window
from kivy.uix.textinput import TextInput
from kivy.uix.checkbox import CheckBox
from kivy.uix.scrollview import ScrollView  
from kivy.clock import Clock
from kivy.uix.popup import Popup
from kivy.uix.spinner import Spinner
from kivy.core.text import LabelBase
from datetime import datetime, timedelta
from kivy.uix.gridlayout import GridLayout
from kivy.core.audio import SoundLoader

# 전체 배경색을 흰색으로 설정
Window.clearcolor = (1, 1, 1, 1)

# 폰트 경로 설정 및 폰트 등록
font_path = 'C:/Users/ah517/Downloads/clova-all/clova-all/희망누리/나눔손글씨 희망누리.ttf'
LabelBase.register(name='NanumHandwriting', fn_regular=font_path)  # 폰트 등록


# 메뉴 화면 정의

class MenuScreen(Screen):
    def __init__(self, **kwargs):
        super(MenuScreen, self).__init__(**kwargs)
        
        layout = BoxLayout(orientation='vertical', padding=50, spacing=20)

        title_label = Label(text="계획쟁이", size_hint_y=None, height=100, font_size=100, font_name='NanumHandwriting',color=(0,0,0,1))
        layout.add_widget(title_label)

        layout.add_widget(Label(text='메인 메뉴', size_hint_y=None, height=50, font_size=30, font_name='NanumHandwriting'))

        # 각 버튼 생성 및 이벤트 연결
        task_button = Button(text='할 일 기록하기', size_hint_y=None, height=80, font_name='NanumHandwriting')
        task_button.bind(on_release=self.go_to_todo)
        layout.add_widget(task_button)

        timer_button = Button(text='타이머', size_hint_y=None, height=80, font_name='NanumHandwriting')
        timer_button.bind(on_release=self.go_to_timer)
        layout.add_widget(timer_button)

        stopwatch_button = Button(text='스톱워치', size_hint_y=None, height=80, font_name='NanumHandwriting')
        stopwatch_button.bind(on_release=self.go_to_stopwatch)
        layout.add_widget(stopwatch_button)

        self.add_widget(layout)

    def go_to_todo(self, instance):
        self.manager.current = 'todo'

    def go_to_timer(self, instance):
        self.manager.current = 'timer'

    def go_to_stopwatch(self, instance):
        self.manager.current = 'stopwatch'

class TodoScreen(Screen):
    def __init__(self, **kwargs):
        super(TodoScreen, self).__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', padding=50, spacing=20)

        title_label = Label(text='할 일 목록', font_size='24sp', size_hint_y=None, height=50)
        layout.add_widget(title_label)

        self.tasks = []  # 할 일 목록 초기화
        self.last_reset_date = self.get_current_date()  # 마지막 리셋 날짜 초기화

        # 할 일 입력란과 스크롤 뷰 생성
        self.scroll_view = ScrollView(size_hint=(1, None), size=(Window.width, 400))  # 스크롤 가능하게
        self.task_layout = BoxLayout(orientation='vertical', size_hint_y=None)
        self.task_layout.bind(minimum_height=self.task_layout.setter('height'))  # 컨텐츠에 맞춰 높이 조정

        self.scroll_view.add_widget(self.task_layout)
        layout.add_widget(self.scroll_view)

        # 추가 버튼
        add_button = Button(text='추가 +', size_hint_y=None, height=50, font_name='NanumHandwriting')
        add_button.bind(on_release=self.add_task)  # 버튼 클릭 시 입력란 추가
        layout.add_widget(add_button)

        # 뒤로 가기 버튼
        back_button = Button(text='뒤로', size_hint_y=None, height=50, font_name='NanumHandwriting')
        back_button.bind(on_release=self.go_back)
        layout.add_widget(back_button)

        self.add_widget(layout)

        # 처음 6개의 입력란 추가
        for _ in range(6):
            self.add_task()

        self.check_reset()  # 앱 시작 시 리셋 확인

    def go_back(self, instance):
        self.manager.current = 'menu'

    def add_task(self, instance=None):
        task_box = BoxLayout(size_hint_y=None, height=50)

        checkBox = CheckBox(size_hint_x=None, width=40)  # 체크박스 생성
        task_input = TextInput(hint_text='할 일을 입력하세요', multiline=False, size_hint_x=0.7, font_name='NanumHandwriting')  # 입력란 생성

        delete_button = Button(text='-', size_hint_x=None, width=40, font_name='NanumHandwriting')
        delete_button.bind(on_release=lambda x: self.remove_task(task_box))  # 삭제 버튼 클릭 시 해당 박스 삭제

        task_box.add_widget(checkBox)
        task_box.add_widget(task_input)
        task_box.add_widget(delete_button)

        self.task_layout.add_widget(task_box)  # 입력란 추가

        self.tasks.append({'task_input': task_input, 'checkBox': checkBox})  # 할 일 목록에 추가

    def remove_task(self, task_box):
        self.task_layout.remove_widget(task_box)  # 해당 입력란 및 체크박스 삭제

    def check_reset(self):
        current_date = self.get_current_date()
        if current_date != self.last_reset_date:
            self.tasks.clear()  # 할 일 목록 리셋
            self.task_layout.clear_widgets()  # UI에서 모든 위젯 제거
            self.last_reset_date = current_date  # 마지막 리셋 날짜 업데이트
            print("할 일 목록이 리셋되었습니다.")  # 콘솔에 리셋 알림 출력

            # 초기 6개의 입력란 추가
            for _ in range(6):
                self.add_task()

    def get_current_date(self):
        return datetime.now().date()  # 현재 날짜 반환

# 타이머 화면 정의
class TimerScreen(Screen):
    def __init__(self, **kwargs):
        super(TimerScreen, self).__init__(**kwargs)
        self.alarm_sound = SoundLoader.load("C:/Users/ah517/Downloads/99DD78465CF6B4DE03.mp3")



        self.timer_seconds = 0  # 타이머 초 초기화
        self.is_running = False  # 타이머가 실행 중인지 여부

        layout = BoxLayout(orientation='vertical', padding=20, spacing=10)

        # 타이머 표시 버튼
        self.timer_button = Button(
            text=self.format_time(self.timer_seconds),
            font_size=80,
            size_hint_y=0.3,
            color=(0.1, 0.6, 0.9, 1),
            background_color=(0, 0, 0, 0),  # 배경 투명
            font_name='NanumHandwriting'  # 폰트 적용
        )
        self.timer_button.bind(on_press=self.open_time_input_popup)  # 버튼 클릭 시 팝업 열기
        layout.add_widget(self.timer_button)

        # 버튼들을 담을 레이아웃 (가로로 정렬)
        button_layout = BoxLayout(size_hint_y=0.2, spacing=20)

        # 타이머 시작/정지 버튼
        self.start_button = Button(text='Start Timer', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        self.start_button.bind(on_press=self.toggle_timer)
        button_layout.add_widget(self.start_button)

        # 10분 추가 버튼
        add_button = Button(text='+ 10분 추가', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        add_button.bind(on_press=self.add_ten_minutes)
        button_layout.add_widget(add_button)

        # 뒤로 가기 버튼
        back_button = Button(text='뒤로', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        back_button.bind(on_press=self.go_back)
        button_layout.add_widget(back_button)

        layout.add_widget(button_layout)

        # 타이머 업데이트 (1초마다)
        Clock.schedule_interval(self.update_timer, 1)

        self.add_widget(layout)  # 레이아웃을 화면에 추가하는 부분

    def open_time_input_popup(self, instance):
        """시간 설정을 위한 팝업 열기"""
        hour_spinner = Spinner(
            text='00',
            values=[f'{i:02}' for i in range(1, 13)],  # 1~12시
            size_hint=(0.2, None),
            height=40,
            font_name='NanumHandwriting'
        )

        minute_spinner = Spinner(
            text='00',
            values=[f'{i:02}' for i in range(0, 60)],  # 00~59분
            size_hint=(0.2, None),
            height=40,
            font_name='NanumHandwriting'
        )
        
        second_spinner = Spinner(
            text='00',
            values=[f'{i:02}' for i in range(0, 60)],
            size_hint = (0.2, None),
            height = 40,
            font_name='NanumHandwriting'
        )

        hour_label = Label(text='시', size_hint=(None, None), size=(30, 40), font_name='NanumHandwriting')
        minute_label = Label(text='분', size_hint=(None, None), size=(30, 40), font_name='NanumHandwriting')
        second_label = Label(text='초', size_hint=(None, None), size=(30, 40), font_name='NanumHandwriting')
        
        layout = BoxLayout(orientation='horizontal', padding=10, spacing=10)
        layout.add_widget(hour_spinner)
        layout.add_widget(hour_label)
        layout.add_widget(minute_spinner)
        layout.add_widget(minute_label)
        layout.add_widget(second_spinner) 
        layout.add_widget(second_label)
        
        save_button = Button(text='저장', size_hint_y=None, height=40, font_name='NanumHandwriting')
   
        save_button.bind(on_release=lambda x: self.set_timer(hour_spinner.text, minute_spinner.text, second_spinner.text))

        layout.add_widget(save_button)
        popup = Popup(title='타이머 설정', content=layout, size_hint=(0.6, 0.4))
        popup.open()

    def set_timer(self, hours, minutes, seconds):
        """타이머 설정 메소드"""
        self.timer_seconds = int(hours) * 3600 + int(minutes) * 60 + int(seconds)  # 초로 변환
        self.timer_button.text = self.format_time(self.timer_seconds)

    def format_time(self, seconds):
        """초를 시:분:초 형식으로 변환"""
        hours, remainder = divmod(seconds, 3600)
        minutes, seconds = divmod(remainder, 60)
        return f'{hours:02}:{minutes:02}:{seconds:02}'

    def toggle_timer(self, instance):
        """타이머 시작 또는 정지"""
        self.is_running = not self.is_running  # 상태 전환
        self.start_button.text = '정지' if self.is_running else '시작'  # 버튼 텍스트 변경
        if self.is_running:
            Clock.schedule_interval(self.update_timer,1)
        else:
            Clock.unschedule(self.update_timer)
            
    def update_timer(self, dt):
        """타이머 업데이트 메소드"""
        if self.is_running and self.timer_seconds > 0:
            self.timer_seconds -= 1  # 초 감소
            self.timer_button.text = self.format_time(self.timer_seconds)
            if self.timer_seconds == 0:
                self.is_running = False  # 타이머가 끝났을 때 정지
                self.start_button.text = '시작'  # 버튼 텍스트 초기화
                if self.alarm_sound:
                    self.alarm_sound.play()

    def add_ten_minutes(self, instance):
        """10분 추가 메소드"""
        self.timer_seconds += 600  # 10분(600초) 추가
        self.timer_button.text = self.format_time(self.timer_seconds)

    def go_back(self, instance):
        """메뉴로 돌아가기"""
        self.manager.current = 'menu'

# 스톱워치 화면 정의
class StopwatchScreen(Screen):
    def __init__(self, **kwargs):
        super(StopwatchScreen, self).__init__(**kwargs)
        
        self.elapsed_time = 0  # 경과 시간 초기화
        self.is_running = False  # 스톱워치가 실행 중인지 여부

        layout = BoxLayout(orientation='vertical', padding=20, spacing=10)

        # 스톱워치 표시 버튼
        self.stopwatch_button = Button(
            text=self.format_time(self.elapsed_time),
            font_size=80,
            size_hint_y=0.3,
            color=(0.1, 0.6, 0.9, 1),
            background_color=(0, 0, 0, 0),  # 배경 투명
            font_name='NanumHandwriting'  # 폰트 적용
        )
        self.stopwatch_button.bind(on_press=self.toggle_stopwatch)  # 버튼 클릭 시 스톱워치 시작/정지
        layout.add_widget(self.stopwatch_button)

        # 버튼들을 담을 레이아웃 (가로로 정렬)
        button_layout = BoxLayout(size_hint_y=0.2, spacing=20)

        # 스톱워치 시작/정지 버튼
        self.start_button = Button(text='Start Stopwatch', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        self.start_button.bind(on_press=self.toggle_stopwatch)
        button_layout.add_widget(self.start_button)

        # 스톱워치 초기화 버튼
        reset_button = Button(text='초기화', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        reset_button.bind(on_press=self.reset_stopwatch)
        button_layout.add_widget(reset_button)

        # 뒤로 가기 버튼
        back_button = Button(text='뒤로', size_hint=(0.3, 1), font_size=30, font_name='NanumHandwriting')
        back_button.bind(on_press=self.go_back)
        button_layout.add_widget(back_button)

        layout.add_widget(button_layout)

        # 경과 시간 업데이트 (1초마다)
        Clock.schedule_interval(self.update_stopwatch, 1)

        self.add_widget(layout)

    def toggle_stopwatch(self, instance):
        """스톱워치 시작 또는 정지"""
        self.is_running = not self.is_running  # 상태 전환
        self.start_button.text = '정지' if self.is_running else '시작'  # 버튼 텍스트 변경

    def update_stopwatch(self, dt):
        """스톱워치 업데이트 메소드"""
        if self.is_running:
            self.elapsed_time += 1  # 1초 증가
            self.stopwatch_button.text = self.format_time(self.elapsed_time)  # 텍스트 업데이트

    def format_time(self, seconds):
        """초를 시:분:초 형식으로 변환"""
        hours, remainder = divmod(seconds, 3600)
        minutes, seconds = divmod(remainder, 60)
        return f'{hours:02}:{minutes:02}:{seconds:02}'

    def reset_stopwatch(self, instance):
        """스톱워치 초기화 메소드"""
        self.elapsed_time = 0  # 경과 시간 초기화
        self.stopwatch_button.text = self.format_time(self.elapsed_time)

    def go_back(self, instance):
        """메뉴로 돌아가기"""
        self.manager.current = 'menu'



class TodoApp(App):
    def build(self):
        sm = ScreenManager()
        sm.add_widget(MenuScreen(name='menu'))
        sm.add_widget(TodoScreen(name='todo'))
        sm.add_widget(TimerScreen(name='timer'))
        sm.add_widget(StopwatchScreen(name='stopwatch'))
        
        return sm

if __name__ == '__main__':
    TodoApp().run()
