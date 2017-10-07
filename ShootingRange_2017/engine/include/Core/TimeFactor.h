#ifndef _TIMEFACTOR_H_
#define _TIMEFACTOR_H_

namespace Core {

//
//  ласс-множитель времени, умеющий замедл€ть и ускор€ть врем€.
//
class TimeFactor {
public:
	//
	//  онструктор по умолчанию
	// 
	TimeFactor();

	//
	// ¬озвращает значение множител€ времени
	//
	float GetValue() const;

	//
	// ¬ернуть строковое значение множител€ времени
	// 
	std::string ToString() const;

	//
	// ”величить множитель времени (ускорить всЄ)
	//
	void Increase();

	//
	// ”меньшить множитель времени (замедлить всЄ)
	//
	void Decrease();

	//
	// ¬озвращает количество множителей времени
	//
	size_t GetScalesCount() const;

	//
	// ¬озвращает индекс текущего множител€ времени
	//
	size_t GetCurrentScaleIndex() const;

	//
	// ”станавливает индекс текущего множител€ времени
	//
	void SetCurrentScaleIndex(size_t index);

	//
	// ¬озвращает значение указанного множител€ времени
	//
	float GetScaleValue(size_t index) const;

	//
	// ¬ернуть множитель времени к значению по-умолчанию (1.0)
	//
	void Reset();

private:
	std::vector<float> _scales;
		// массив множителей времени

	const size_t DEFAULT_SCALE_INDEX;
		// индекс массива множителей по умолчанию

	size_t _currentScaleIndex;
		// текущий индекс массива множителей
};

extern TimeFactor timeFactor;

} // namespace Core

#endif
