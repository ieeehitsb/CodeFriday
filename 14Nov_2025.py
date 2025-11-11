from manim import *
import numpy as np

def animated_title_mobject(title="Code Friday", font_size=64):
    return Text(title, font_size=font_size)

def bubble_sort_bars(values, bar_width=0.5, max_height=3):
    vals = list(values)
    max_val = max(vals) if vals else 1
    bars = VGroup()
    for i, v in enumerate(vals):
        height = (v / max_val) * max_height
        rect = Rectangle(width=bar_width, height=height, fill_opacity=1).set_fill(BLUE)
        rect.move_to(np.array([i * (bar_width + 0.15), -max_height / 2 + height / 2, 0]))
        label = Text(str(v)).scale(0.5).next_to(rect, DOWN)
        g = VGroup(rect, label)
        g.value = int(v)
        bars.add(g)
    return bars

def fibonacci_sequence_group(n=8):
    seq = [0, 1]
    for _ in range(2, n):
        seq.append(seq[-1] + seq[-2])
    nodes = VGroup(*[Text(str(x)).scale(0.7) for x in seq])
    nodes.arrange(RIGHT, buff=0.6)
    return nodes

def plot_equation_graph(func=lambda x: np.sin(x), x_range=(-PI, PI), width=6, height=3):
    axes = Axes(
        x_range=[x_range[0], x_range[1], PI/2],
        y_range=[-2, 2, 1],
        x_length=width, y_length=height
    )
    graph = axes.plot(func, x_range=x_range)
    label = Text("f(x)").scale(0.6).next_to(axes, UP)
    return VGroup(axes, graph, label)

def matrix_transform_demo(square_size=1.2, matrix=[[0, -1], [1, 0]]):
    square = Square(side_length=square_size).set_fill(YELLOW, 0.6)
    mat = np.array(matrix)
    return square, mat

def demo_title(scene, text="Manim Library", size=48, hold=0.4):
    t = animated_title_mobject(text, font_size=size)
    scene.play(Write(t))
    scene.wait(hold)
    scene.play(t.animate.to_edge(UP).scale(0.85))

def demo_bubble_sort(scene, values=[5,2,9,1,6], hold=0.15):
    bars = bubble_sort_bars(values)
    bars.move_to(ORIGIN)
    scene.play(FadeIn(bars))
    n = len(bars)
    for i in range(n):
        for j in range(0, n - i - 1):
            left = bars[j]; right = bars[j+1]
            if getattr(left, "value", 0) > getattr(right, "value", 0):
                scene.play(AnimationGroup(
                    left.animate.shift(RIGHT * 0.65),
                    right.animate.shift(LEFT * 0.65),
                    run_time=0.45
                ))
                bars[j], bars[j+1] = bars[j+1], bars[j]
                scene.wait(0.03)
    scene.wait(hold)
    scene.play(FadeOut(bars))

def demo_fibonacci(scene, n=6, hold=0.12):
    fib = fibonacci_sequence_group(n)
    fib.move_to(ORIGIN)
    scene.play(Write(fib))
    for i in range(2, len(fib)):
        box1 = SurroundingRectangle(fib[i-2], buff=0.12).set_stroke(TEAL, 3)
        box2 = SurroundingRectangle(fib[i-1], buff=0.12).set_stroke(GREEN, 3)
        scene.play(Create(box1), Create(box2), run_time=0.18)
        scene.wait(0.06)
        scene.play(FadeOut(box1), FadeOut(box2))
    scene.wait(hold)
    scene.play(FadeOut(fib))

def demo_plot(scene, func=lambda x: np.sin(x), rng=(-PI, PI), hold=0.12):
    pg = plot_equation_graph(func, x_range=rng)
    pg.move_to(ORIGIN)
    scene.play(Create(pg[0]), Create(pg[1]))
    axes, graph = pg[0], pg[1]
    dot = Dot(color=RED).move_to(axes.c2p(rng[0], func(rng[0])))
    scene.play(FadeIn(dot))
    scene.play(MoveAlongPath(dot, graph), run_time=1.2)
    scene.play(FadeOut(dot), FadeOut(pg))

def demo_matrix_transform(scene, hold=0.12):
    square, mat = matrix_transform_demo()
    square.move_to(ORIGIN)
    scene.play(FadeIn(square))
    scene.play(square.animate.apply_matrix(mat), run_time=0.9)
    mat_text = Text(f"{int(mat[0,0])} {int(mat[0,1])}\n{int(mat[1,0])} {int(mat[1,1])}").scale(0.6)
    mat_text.next_to(square, RIGHT, buff=0.4)
    scene.play(Write(mat_text))
    scene.wait(hold)
    scene.play(FadeOut(square), FadeOut(mat_text))

class LibraryShowcaseMini(Scene):
    def construct(self):
        demo_title(self, "Manim Library", size=48, hold=0.25)
        demo_bubble_sort(self, [5,2,9,1,6], hold=0.08)
        demo_fibonacci(self, n=6, hold=0.08)
        demo_plot(self, func=lambda x: np.sin(x), rng=(-PI, PI), hold=0.08)
        demo_matrix_transform(self, hold=0.08)
