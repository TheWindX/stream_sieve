#lang racket
;stream
(define-syntax-rule (mdelay exp)
    (let ((v null))
      (lambda ()
        (if (null? v)
            (begin (set! v exp) v)
            v))))

(define-syntax-rule (mforce lv)
   (lv))

(define-syntax-rule (cons-stream a b)
  (cons a (mdelay b)))

(define (stream-car stream)
  (car stream))

(define (stream-cdr stream)
  (mforce (cdr stream)))

(define (make-mstream init fnext)
  (cons-stream init (make-mstream (fnext init) fnext)))

(define (rangefrom n)
  (cons-stream n (rangefrom (+ n 1))))

(define integers (rangefrom 1))

(define (stream-filter stream pred)
  (let find ((test-stream stream))
    (let ((first (stream-car test-stream)))
      (if (pred first)
        (cons-stream first 
                     (stream-filter (stream-cdr test-stream) pred))
        (find (stream-cdr test-stream))))))

(define (print_stream stream n)
  (let loop ((i  n) 
             (first (stream-car stream) )
             (rest (stream-cdr stream)))
    (if (eq? i 0)
        null
        (if (eq? rest null)
            null
            (begin (displayln first)
                   (loop (- i 1) (stream-car rest) (stream-cdr rest)))))))
             
;now let's find primary
(define (sieve stream)
  (cons-stream
   (stream-car stream)
   (sieve (stream-filter stream
                         (lambda (n) (not (eq? 0 (modulo n (stream-car stream)))))))))

(print_stream (sieve (rangefrom 2)) 15)