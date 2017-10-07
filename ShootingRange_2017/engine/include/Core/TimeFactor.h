#ifndef _TIMEFACTOR_H_
#define _TIMEFACTOR_H_

namespace Core {

//
// �����-��������� �������, ������� ��������� � �������� �����.
//
class TimeFactor {
public:
	//
	// ����������� �� ���������
	// 
	TimeFactor();

	//
	// ���������� �������� ��������� �������
	//
	float GetValue() const;

	//
	// ������� ��������� �������� ��������� �������
	// 
	std::string ToString() const;

	//
	// ��������� ��������� ������� (�������� ��)
	//
	void Increase();

	//
	// ��������� ��������� ������� (��������� ��)
	//
	void Decrease();

	//
	// ���������� ���������� ���������� �������
	//
	size_t GetScalesCount() const;

	//
	// ���������� ������ �������� ��������� �������
	//
	size_t GetCurrentScaleIndex() const;

	//
	// ������������� ������ �������� ��������� �������
	//
	void SetCurrentScaleIndex(size_t index);

	//
	// ���������� �������� ���������� ��������� �������
	//
	float GetScaleValue(size_t index) const;

	//
	// ������� ��������� ������� � �������� ��-��������� (1.0)
	//
	void Reset();

private:
	std::vector<float> _scales;
		// ������ ���������� �������

	const size_t DEFAULT_SCALE_INDEX;
		// ������ ������� ���������� �� ���������

	size_t _currentScaleIndex;
		// ������� ������ ������� ����������
};

extern TimeFactor timeFactor;

} // namespace Core

#endif
